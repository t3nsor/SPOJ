// 2025-04-08
// Observation: we can colour the tree using only two colours: 0 (boss) and 1
// (non-boss); the value of `M` is important only in determining whether two
// non-boss nodes are in the same group. If `M` is at least 3, we can
// guarantee that two adjacent non-boss nodes are never in the same group (so
// no one has to eat the edge between them).
#include <algorithm>
#include <iostream>
#include <string.h>
#include <utility>
#include <vector>
using namespace std;
// dp[i][j][m] = min illness to eat all fruits in `i`'s subtree where boss eats
//               `j` fruits total and fruit `i` is eaten by `m`
int dp[300][301][2];
int N, M, K;
vector<vector<pair<int, int>>> adj;
vector<int> sz;
vector<int> pain;
vector<vector<int>> children;
void dfs(int u, int p) {
    sz[u] = 1;
    for (const auto& entry : adj[u]) {
        if (entry.first != p) {
            pain[entry.first] = entry.second;
            children[u].push_back(entry.first);
            dfs(entry.first, u);
            sz[u] += sz[entry.first];
        }
    }
    // merge from smallest to largest, possibly micro-optimization
    sort(children[u].begin(), children[u].end(),
         [](int c1, int c2) { return sz[c1] < sz[c2]; });
}
void rec(int root) {
    for (auto child : children[root]) rec(child);

    {
        // try with m == 0
        int scratch[301];
        memset(scratch, 0x3f, sizeof(scratch));
        scratch[1] = 0;
        int maxj = 1;
        for (auto child : children[root]) {
            int newmaxj = min(maxj + sz[child], K);
            for (int j = newmaxj; j >= 0; j--) {
                int val = 0x3f3f3f3f;
                for (int k = 0; k <= min({j, sz[child], K}); k++) {
                    val = min(val, scratch[j - k] + dp[child][k][0] +
                                   pain[child]);
                    val = min(val, scratch[j - k] + dp[child][k][1]);
                }
                scratch[j] = val;
            }
            maxj = newmaxj;
        }
        for (int j = 0; j <= K; j++) dp[root][j][0] = scratch[j];
    }
    if (root != 0) {
        // try with m == 1
        int scratch[301];
        memset(scratch, 0x3f, sizeof(scratch));
        scratch[0] = 0;
        int maxj = 0;
        for (auto child : children[root]) {
            int newmaxj = min(maxj + sz[child], K);
            for (int j = newmaxj; j >= 0; j--) {
                int val = 0x3f3f3f3f;
                for (int k = 0; k <= min({j, sz[child], K}); k++) {
                    val = min(val, scratch[j - k] + dp[child][k][0]);
                    val = min(val, scratch[j - k] + dp[child][k][1] +
                                   (M == 2 ? pain[child] : 0));
                }
                scratch[j] = val;
            }
            maxj = newmaxj;
        }
        for (int j = 0; j <= K; j++) dp[root][j][1] = scratch[j];
    }
}
void do_testcase() {
    cin >> N >> M >> K;
    adj.assign(N, {});
    for (int i = 0; i < N - 1; i++) {
        int u, v, w; cin >> u >> v >> w; --u; --v;
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
    }
    if (K + (M - 1) > N) { cout << "-1\n"; return; }
    children.assign(N, {});
    sz.assign(N, 0);
    pain.assign(N, 0);
    dfs(0, -1);
    rec(0);
    cout << dp[0][K][0] << '\n';
}
int main() {
    for (int i = 0; i < 10; i++) do_testcase();
}
