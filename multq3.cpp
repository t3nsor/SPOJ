// 2014-10-01
#include <cstdio>
#include <vector>
using namespace std;
struct Segtree {
    int n;
    struct Node {
        int cnt[3];
        int sum;
    };
    vector<Node> data;
    void combine(Node& left, const Node& right) {
        for (int i = 0; i < 3; i++) {
            int s = i + left.sum;
            if (s >= 3) s -= 3;
            left.cnt[s] += right.cnt[i];
        }
        left.sum += right.sum;
        if (left.sum >= 3) left.sum -= 3;
    }

    void build_rec(int node, int begin, int end) {
        if (end == begin+1) {
            if (data.size() <= node) data.resize(node+1);
        } else {
            int mid = begin + (end-begin+1)/2;
            build_rec(2*node+1, begin, mid);
            build_rec(2*node+2, mid, end);
        }
        data[node].cnt[0] = end - begin;
    }

    void update_rec(int node, int begin, int end, int pos, int val) {
        if (end == begin+1) {
            int m = val%3;
            if (m < 0) m += 3;
            data[node].sum = m;
            for (int i = 0; i < 3; i++) {
                data[node].cnt[i] = (i == m);
            }
        } else {
            int mid = begin + (end-begin+1)/2;
            if (pos < mid) {
                update_rec(2*node+1, begin, mid, pos, val);
            } else {
                update_rec(2*node+2, mid, end, pos, val);
            }
            data[node] = data[2*node+1];
            combine(data[node], data[2*node+2]);
        }
    }

    void query_rec(Node& res, int node, int tbegin, int tend, int abegin, int aend) {
        if (tbegin >= abegin && tend <= aend) {
            res = data[node];
        } else {
            int mid = tbegin + (tend - tbegin + 1)/2;
            if (mid > abegin && tbegin < aend) {
                query_rec(res, 2*node+1, tbegin, mid, abegin, aend);
                if (tend > abegin && mid < aend) {
                    Node right;
                    query_rec(right, 2*node+2, mid, tend, abegin, aend);
                    combine(res, right);
                }
            } else if (tend > abegin && mid < aend) {
                query_rec(res, 2*node+2, mid, tend, abegin, aend);
            } else {
                res = Node();
            }
        }
    }

    Segtree(int n): n(n) {
        build_rec(0, 0, n);
    }

    // Call this to update a value (indices are 0-based). If lazy propagation is enabled, use update_range(pos, pos+1, val) instaed.
    void update(int pos, int val) {
        update_rec(0, 0, n, pos, val);
    }

    // Returns minimum in range [begin, end). Indices are 0-based.
    void query(Node& res, int begin, int end) {
        return query_rec(res, 0, 0, n, begin, end);
    }
};

int main() {
    int N, Q; scanf("%d %d", &N, &Q);
    Segtree T(N);
    static int diff[100000];
    while (Q--) {
        int cmd, a, b;
        scanf("%d %d %d", &cmd, &a, &b);
        if (cmd == 0) {
            T.update(a, ++diff[a]);
            if (b < N-1) {
                T.update(b+1, --diff[b+1]);
            }
        } else {
            Segtree::Node pre; T.query(pre, 0, a);
            Segtree::Node res; T.query(res, a, b+1);
            printf("%d\n", res.cnt[(3-pre.sum)%3]);
        }
    }
    return 0;
}
