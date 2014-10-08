// 2014-10-02
// Uses a segtree with lazy propagation to repeatedly locate the
// highest ranking remaining soldier.
// TODO: There's an easier solution with BIT.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Segtree {
    int n;
    vector<int> data;
#define NOLAZY 0
#define GET(node) (data[node] + lazy[node])
    vector<int> lazy;
    void build_rec(int node, int* begin, int* end) {
        if (end == begin+1) {
            if (data.size() <= node) data.resize(node+1);
            data[node] = *begin;
        } else {
            int* mid = begin + (end-begin+1)/2;
            build_rec(2*node+1, begin, mid);
            build_rec(2*node+2, mid, end);
            data[node] = min(data[2*node+1], data[2*node+2]);
        }
    }
    void update_range_rec(int node, int tbegin, int tend, int abegin, int aend, int val) {
        if (tbegin >= abegin && tend <= aend) {
            lazy[node] += val;
        } else {
            int mid = tbegin + (tend - tbegin + 1)/2;
            if (lazy[node] != NOLAZY) {
                lazy[2*node+1] += lazy[node];
                lazy[2*node+2] += lazy[node];
                lazy[node] = NOLAZY;
            }
            if (mid > abegin && tbegin < aend)
                update_range_rec(2*node+1, tbegin, mid, abegin, aend, val);
            if (tend > abegin && mid < aend)
                update_range_rec(2*node+2, mid, tend, abegin, aend, val);
            data[node] = min(GET(2*node+1), GET(2*node+2));
        }
    }
    int find_rightmost_zero_rec(int node, int begin, int end) {
        if (end == begin + 1) {
            return begin;
        } else {
            if (lazy[node] != NOLAZY) {
                data[node] += lazy[node];
                lazy[2*node+1] += lazy[node];
                lazy[2*node+2] += lazy[node];
                lazy[node] = NOLAZY;
            }
            int mid = begin + (end - begin + 1)/2;
            if (GET(2*node+2) == 0) {
                return find_rightmost_zero_rec(2*node+2, mid, end);
            } else {
                return find_rightmost_zero_rec(2*node+1, begin, mid);
            }
        }
    }

    // Create a segtree which stores the range [begin, end) in its bottommost level.
    Segtree(int* begin, int* end): n(end - begin) {
        build_rec(0, begin, end);
        lazy.assign(data.size(), NOLAZY);
    }

    // Call this to update range [begin, end), if lazy propagation is enabled. Indices are 0-based.
    void update_range(int begin, int end, int val) {
        update_range_rec(0, 0, n, begin, end, val);
    }
    int find_rightmost_zero() {
        return find_rightmost_zero_rec(0, 0, n);
    }
};

int w[200000];
int a[200000];

int main() {
    int t; scanf("%d", &t);
    while (t--) {
        int n; scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%d", w + i);
        }
        Segtree T(w, w+n);
        for (int i = n; i >= 1; i--) {
            // find rightmost zero
            int pos = T.find_rightmost_zero();
            a[pos] = i;
            T.update_range(pos+1, n, -1);
            T.update_range(pos, pos+1, 1e6);
        }
        for (int i = 0; i < n; i++) {
            printf("%d ", a[i]);
        }
        putchar('\n');
    }
    return 0;
}
