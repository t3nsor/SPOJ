// 2014-06-06
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;
struct Segtree {
    int n;
    typedef long long ll;
    vector<ll> cur;
    vector<ll> best;
    vector<ll> lazy;
    vector<ll> best_lazy;
    Segtree(int n): n(n), cur(4*n), best(4*n), lazy(4*n), best_lazy(4*n) {}
    void discharge(int node) {
        for (int child = 2*node; child <= 2*node+1; child++) {
            best_lazy[child] = max(best_lazy[child],
                                   lazy[child] + best_lazy[node]);
            lazy[child] += lazy[node];
            best[child] = max(best[child],
                              cur[child] + best_lazy[node]);
            cur[child] += lazy[node];
        }
        lazy[node] = best_lazy[node] = 0ll;
    }
    void update_range_rec(int node, int tbegin, int tend, int abegin, int aend, ll val) {
        if (tbegin >= abegin && tend <= aend) {
            best_lazy[node] = max(best_lazy[node], lazy[node] += val);
            best[node] = max(best[node], cur[node] += val);
        } else {
            int mid = tbegin + (tend - tbegin + 1)/2;
            discharge(node);
            if (mid > abegin && tbegin < aend)
                update_range_rec(2*node, tbegin, mid, abegin, aend, val);
            if (tend > abegin && mid < aend)
                update_range_rec(2*node+1, mid, tend, abegin, aend, val);
            cur[node] = max(cur[2*node], cur[2*node+1]);
            best[node] = max(best[node], cur[node]);
        }
    }
    ll query_rec(int node, int tbegin, int tend, int abegin, int aend) {
        if (tbegin >= abegin && tend <= aend) {
            return best[node];
        } else {
            discharge(node);
            int mid = tbegin + (tend - tbegin + 1)/2;
            ll res = numeric_limits<ll>::min();
            if (mid > abegin && tbegin < aend) 
                res = max(res, query_rec(2*node, tbegin, mid, abegin, aend));
            if (tend > abegin && mid < aend)
                res = max(res, query_rec(2*node+1, mid, tend, abegin, aend));
            return res;
        }
    }

    // Call this to update range [begin, end), if lazy propagation is enabled. Indices are 0-based.
    void update_range(int begin, int end, ll val) {
        update_range_rec(1, 0, n, begin, end, val);
    }
    // Returns minimum in range [begin, end). Indices are 0-based.
    ll query(int begin, int end) {
        return query_rec(1, 0, n, begin, end);
    }
};

int main() {
    vector<pair<int, int> > evts;
    int a[100000];
    pair<int, int> q[100000];
    long long ans[100000];
    int last[200001]; memset(last, -1, sizeof last);
    int N; scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d", a+i);
        evts.push_back(make_pair(i, 1e9));
    }
    int Q; scanf("%d", &Q);
    for (int i = 0; i < Q; i++) {
        scanf("%d %d", &q[i].first, &q[i].second);
        q[i].first--;
        evts.push_back(make_pair(q[i].second, i));
    }
    sort(evts.begin(), evts.end());
    Segtree S(N);
    for (int i = 0; i < evts.size(); i++) {
        pair<int, int> evt = evts[i];
        if (evt.second == 1e9) {
            int idx = evt.first;
            // update
            S.update_range(last[a[idx]+100000]+1, idx+1, a[idx]);
            last[a[idx]+100000] = idx;
        } else {
            // query
            int idx = evt.second;
            ans[idx] = S.query(q[idx].first, q[idx].second);
        }
    }
    for (int i = 0; i < Q; i++) {
        printf("%lld\n", ans[i]);
    }
}
