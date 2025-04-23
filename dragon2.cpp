// 2025-04-22
// This solution uses the same algorithm as dragon.cpp but has been rewritten so
// that the inner loop always iterates forward, which makes it about 100 times
// faster for large test cases.  (It also contains a few smaller optimizations.)
// Perhaps surprisingly, the running time is O(N^2). To see this, first observe 
// that it suffices to prove this bound for the case of binary trees, as any
// tree where some nodes have degree more than 2 can be transformed into a
// binary tree where the answer may be different but the running time changes
// by at most a constant factor. We then hypothesize that the number of steps
// spent in the call `rec(u)` (including recursive calls) is bounded by n^2/2
// where n is the subtree size of u and can prove this by induction.  This
// gives an overall upper bound of O(N^2).  It might even be O(NK), I am too
// lazy to prove/disprove this so it's left as an exercise for the reader.
#include <algorithm>
#include <iostream>
#include <string.h>
#include <utility>
#include <vector>
using namespace std;
int dp[3000][3001][2];
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
}
void rec(int root) {
    for (auto child : children[root]) rec(child);

    {
        vector<int> scratch = {0x3f3f3f3f, 0};
        for (auto child : children[root]) {
            vector<int> scratch2(min<int>(scratch.size() + sz[child], K + 1),
                                 0x3f3f3f3f);
            for (int k1 = 0; k1 < scratch.size(); k1++) {
                const int bound = min<int>(scratch2.size() - k1 - 1, sz[child]);
                for (int k2 = 0; k2 <= bound; k2++) {
                    scratch2[k1 + k2] = min({scratch2[k1 + k2],
                                             scratch[k1] + dp[child][k2][0]
                                               + pain[child],
                                             scratch[k1] + dp[child][k2][1]});
                }
            }
            swap(scratch, scratch2);
        }
        for (int j = 0; j < scratch.size(); j++) dp[root][j][0] = scratch[j];
    }
    if (root != 0) {
        vector<int> scratch = {0, 0x3f3f3f3f};
        for (auto child : children[root]) {
            vector<int> scratch2(min<int>(scratch.size() + sz[child], K + 1),
                                 0x3f3f3f3f);
            for (int k1 = 0; k1 < scratch.size(); k1++) {
                const int bound = min<int>(scratch2.size() - k1 - 1, sz[child]);
                for (int k2 = 0; k2 <= bound; k2++) {
                    scratch2[k1 + k2] = min({scratch2[k1 + k2],
                                             scratch[k1] + dp[child][k2][0],
                                             scratch[k1] + dp[child][k2][1]
                                               + (M == 2 ? pain[child] : 0)});
                }
            }
            swap(scratch, scratch2);
        }
        for (int j = 0; j < scratch.size(); j++) dp[root][j][1] = scratch[j];
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
    do_testcase();
}
