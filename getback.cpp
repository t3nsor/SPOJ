// 2026-04-25
// This problem is easier than it looks, and the explanation for the sample test
// case actually gives it away.  The basic question we have to answer is, if you
// need y units of water at node v, and we have to reach v from node u (with a
// road of length d) then how many units of water do we need to start with at
// node u?  And then just work backward from the destination greedily.  I don't
// actually know how to prove that this is optimal, but it intuitively feels
// like it should be.  (Note: According to a comment on the problem description,
// bigints are not needed, so this solution uses `long long`, but I think it
// would also pass with `int`, assuming SPOJ just uses the official test data.)
#include <iostream>
#include <limits.h>
#include <utility>
#include <vector>
using namespace std;
long long do_testcase() {
    int V, E, C; cin >> V >> E >> C;
    vector<vector<pair<int, int>>> adj(V);
    while (E--) {
        int x, y, l; cin >> x >> y >> l; --x; --y;
        adj[x].emplace_back(y, l);
        adj[y].emplace_back(x, l);
    }
    vector<long long> dp(V, LLONG_MAX);
    dp[V-1] = 0;
    vector<int> active(V, 0);
    active[V-1] = 1;
    for (;;) {
        int u = -1; long long best = LLONG_MAX;
        for (int i = 0; i < V; i++) {
            if (active[i] && dp[i] < best) {
                u = i;
                best = dp[i];
            }
        }
        if (u == -1) return -1;
        if (u == 0) return dp[u];
        active[u] = 0;
        for (const auto edge : adj[u]) {
            long long d = edge.second;
            long long x;
            if (dp[u] <= C - d) {
                x = dp[u] + d;
            } else {
                if (2*d >= C) continue;
                long long rt = (dp[u] - (C - d) + C - 2*d - 1) / (C - 2*d);
                long long rta = (C - 2*d)*rt;
                x = C*rt + (dp[u] - rta) + d;
            }
            if (x < dp[edge.first]) {
                dp[edge.first] = x;
                active[edge.first] = 1;
            }
        }
    }
}
int main() {
    int T; cin >> T; while (T--) cout << do_testcase() << '\n';
}
