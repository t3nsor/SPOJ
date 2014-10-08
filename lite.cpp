// 2014-05-03
#include <vector>
#include <cstdio>
using namespace std;
struct Segtree {
    int n;
    vector<int> data;
    vector<bool> lazy;

    void init(int node, int l, int r) {
        if (r == l+1) {
            if (data.size() <= node) data.resize(node+1);
        } else {
            int mid = l + (r - l + 1)/2;
            init(2*node+1, l, mid);
            init(2*node+2, mid, r);
        }
    }

    Segtree(int n): n(n) {
        init(0, 0, n);
        lazy.assign(data.size(), false);
    }

    void update_range_rec(int node, int tbegin, int tend, int abegin, int aend) {
        if (tbegin >= abegin && tend <= aend) {
            lazy[node] = !lazy[node];
        } else {
            int mid = tbegin + (tend - tbegin + 1)/2;
            if (lazy[node]) {
                lazy[2*node+1] = !lazy[2*node+1];
                lazy[2*node+2] = !lazy[2*node+2];
                lazy[node] = false;
            }
            if (mid > abegin && tbegin < aend)
                update_range_rec(2*node+1, tbegin, mid, abegin, aend);
            if (tend > abegin && mid < aend)
                update_range_rec(2*node+2, mid, tend, abegin, aend);
            int lval = lazy[2*node+1] ? (mid - tbegin) - data[2*node+1]
                                      : data[2*node+1];
            int rval = lazy[2*node+2] ? (tend - mid) - data[2*node+2]
                                      : data[2*node+2];
            data[node] = lval + rval;
        }
    }

    int query_rec(int node, int tbegin, int tend, int abegin, int aend) {
        if (tbegin >= abegin && tend <= aend) {
            return lazy[node] ? (tend - tbegin) - data[node] : data[node];
        } else {
            if (lazy[node]) {
                // propagate
                lazy[2*node+1] = !lazy[2*node+1];
                lazy[2*node+2] = !lazy[2*node+2];
                lazy[node] = false;
                data[node] = (tend - tbegin) - data[node];
            }
            int mid = tbegin + (tend - tbegin + 1)/2;
            int res = 0;
            if (mid > abegin && tbegin < aend) 
                res += query_rec(2*node+1, tbegin, mid, abegin, aend);
            if (tend > abegin && mid < aend)
                res += query_rec(2*node+2, mid, tend, abegin, aend);
            return res;
        }
    }

    // Call this to update range [begin, end), if lazy propagation is enabled. Indices are 0-based.
    void update_range(int begin, int end) {
        update_range_rec(0, 0, n, begin, end);
    }

    // Returns minimum in range [begin, end). Indices are 0-based.
    int query(int begin, int end) {
        return query_rec(0, 0, n, begin, end);
    }
};

int main() {
    int N, M; scanf("%d %d", &N, &M);
    Segtree B(N);
    while (M--) {
        int type, x, y;
        scanf("%d %d %d", &type, &x, &y);
        if (type == 0) {
            // update
            B.update_range(x-1, y);
        } else {
            // query
            printf("%d\n", B.query(x-1, y));
        }
    }
}
