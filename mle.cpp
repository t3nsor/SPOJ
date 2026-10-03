// 2026-10-02
// I don't know why this problem is called MLE.  It's not especially demanding
// of memory compared to other problems.  Maybe they called it that just to make
// it harder for people to google it.
//
// The most obvious approach to obtain guaranteed O(n log n) runtime, which is
// also unfortunately very complex to implement, is to compute the Voronoi
// diagram; for a given point P the closest other point Q must have a Voronoi
// cell that borders P's Voronoi cell.  The solution used here is a k-d tree,
// which is much simpler to code and gives O(n log n) average case performance;
// we use some randomization to avoid the worst case.  There are some other
// approaches discussed here: https://codeforces.com/blog/entry/45583

#include <algorithm>
#include <limits.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <vector>
using namespace std;
using LL = long long;
mt19937 engine(2009575068);

enum NodeType {
    SPLIT_X,  // `split_val` is x coordinate at which to split
    SPLIT_Y,  // `split_val` is y coordinate at which to split
    LEAF,
};

struct Node {
    NodeType type;
    union {
        // yes I know anonymous structs are not standard C++ but since I do use
        // __int128 for some problems it's not like I'm pure
        struct { int split_val; int child[2]; };
        int pt[3];
    };
};

Node tree[300000];
int next_node;
int X[100000], Y[100000];

void build_tree(int node, NodeType next_type,
                vector<int> xlist, vector<int> ylist) {
    const int m = xlist.size();
    if (m <= 3) {
        tree[node].type = LEAF;
        for (int i = 0; i < 3; i++) {
            tree[node].pt[i] = i < m ? xlist[i] : -1;
        }
        return;
    }
    const int adjust = uniform_int_distribution<int>(-m/10, m/10)(engine);
    const int split_rank = m/2 + adjust;
    tree[node].type = next_type;
    tree[node].child[0] = -1;
    tree[node].child[1] = -1;
    vector<int> x1, y1, x2, y2;
    if (next_type == SPLIT_X) {
        next_type = SPLIT_Y;
        const int sv = tree[node].split_val = X[xlist[split_rank]];
        for (const auto i : xlist) {
            if (X[i] < sv) x1.push_back(i); else x2.push_back(i);
        }
        for (const auto i : ylist) {
            if (X[i] < sv) y1.push_back(i); else y2.push_back(i);
        }
    } else {
        next_type = SPLIT_X;
        const int sv = tree[node].split_val = Y[ylist[split_rank]];
        for (const auto i : xlist) {
            if (Y[i] < sv) x1.push_back(i); else x2.push_back(i);
        }
        for (const auto i : ylist) {
            if (Y[i] < sv) y1.push_back(i); else y2.push_back(i);
        }
    }
    xlist.clear();
    ylist.clear();
    if (!x1.empty()) {
        tree[node].child[0] = next_node++;
        build_tree(next_node - 1, next_type, move(x1), move(y1));
    }
    if (!x2.empty()) {
        tree[node].child[1] = next_node++;
        build_tree(next_node - 1, next_type, move(x2), move(y2));
    }
}

LL sqr(LL x) { return x*x; }

LL rect_sqrdist(LL px, LL py, LL x1, LL y1, LL x2, LL y2) {
    return sqr((abs(px - x1) + abs(px - x2) - (x2 - x1)) / 2) +
           sqr((abs(py - y1) + abs(py - y2) - (y2 - y1)) / 2);
}

LL min_sqrdist(int p, int node, int x1, int y1, int x2, int y2, LL cur) {
    if (tree[node].type == LEAF) {
        for (const int pt : tree[node].pt) {
            if (pt >= 0 && pt != p) {
                cur = min(cur, sqr(X[p] - X[pt]) + sqr(Y[p] - Y[pt]));
            }
        }
        return cur;
    }
    LL rdist[2] = {LLONG_MAX, LLONG_MAX};
    int x1_1 = x1, y1_1 = y1, x2_1, y2_1;
    int x1_2, y1_2, x2_2 = x2, y2_2 = y2;
    if (tree[node].type == SPLIT_X) {
        x2_1 = tree[node].split_val; x1_2 = x2_1 - 1;
        y2_1 = y2; y1_2 = y1;
    } else {
        y2_1 = tree[node].split_val; y1_2 = y2_1 - 1;
        x2_1 = x2; x1_2 = x1;
    }
    if (tree[node].child[0] >= 0) {
        rdist[0] = rect_sqrdist(X[p], Y[p], x1_1, y1_1, x2_1, y2_1);
    }
    if (tree[node].child[1] >= 0) {
        rdist[1] = rect_sqrdist(X[p], Y[p], x1_2, y1_2, x2_2, y2_2);
    }
    if (rdist[0] < rdist[1]) {
        if (rdist[0] < cur) {
            cur = min(cur, min_sqrdist(p, tree[node].child[0],
                                       x1_1, y1_1, x2_1, y2_1, cur));
            if (rdist[1] < cur) {
                cur = min(cur, min_sqrdist(p, tree[node].child[1],
                                           x1_2, y1_2, x2_2, y2_2, cur));
            }
        }
    } else {
        if (rdist[1] < cur) {
            cur = min(cur, min_sqrdist(p, tree[node].child[1],
                                       x1_2, y1_2, x2_2, y2_2, cur));
            if (rdist[0] < cur) {
                cur = min(cur, min_sqrdist(p, tree[node].child[0],
                                           x1_1, y1_1, x2_1, y2_1, cur));
            }
        }
    }
    return cur;
}

void do_testcase() {
    int n; scanf("%d", &n);
    vector<int> xlist(n), ylist(n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", X + i, Y + i);
        // check for potential overflow
        if (abs(X[i]) >= (1 << 30) || abs(Y[i]) >= (1 << 30)) throw;
        xlist[i] = ylist[i] = i;
    }
    sort(xlist.begin(), xlist.end(),
         [&](int i, int j) { return X[i] < X[j]; });
    sort(ylist.begin(), ylist.end(),
         [&](int i, int j) { return Y[i] < Y[j]; });
    next_node = 1;
    build_tree(0, SPLIT_X, move(xlist), move(ylist));
    for (int i = 0; i < n; i++) {
        printf("%lld\n", min_sqrdist(i, 0,
                                     INT_MIN, INT_MIN, INT_MAX, INT_MAX,
                                     LLONG_MAX));
    }
}

int main() {
    int T; scanf("%d", &T); while(T--) do_testcase();
}
