// 2023-04-17
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
void do_testcase() {
    int V, E, b; cin >> V >> E >> b; --b;
    vector<vector<int>> dist(V, vector<int>(V, 1e8));
    for (int i = 0; i < V; i++) dist[i][i] = 0;
    while (E--) {
        int u, v, d; cin >> u >> v >> d; --u; --v;
        dist[u][v] = min(dist[u][v], d);
        dist[v][u] = min(dist[v][u], d);
    }
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
    vector<int> src, dest;
    int z; cin >> z;
    while (z--) {
        int u, v, b; cin >> u >> v >> b; --u; --v;
        while (b--) {
            src.push_back(u);
            dest.push_back(v);
        }
    }
    const int N = src.size();
    if (N == 0) {
        cout << "0\n";
        return;
    }
    vector<vector<int>> dp(1 << N, vector<int>(N, 1e8));
    for (int i = 0; i < N; i++) {
        dp[(1 << i)][i] = dist[b][src[i]] + dist[src[i]][dest[i]];
    }
    for (unsigned i = 0; i < (1 << N); i++) {
        if (__builtin_popcount(i) <= 1) continue;
        for (int j = 0; j < N; j++) {
            if (!(i & (1 << j))) continue;
            const unsigned prev = i - (1 << j);
            for (int k = 0; k < N; k++) {
                if (!(prev & (1 << k))) continue;
                dp[i][j] = min(
                  dp[i][j],
                  dp[prev][k] + dist[dest[k]][src[j]] + dist[src[j]][dest[j]]
                );
            }
        }
    }
    int result = 1e8;
    for (int i = 0; i < N; i++) {
        result = min(result, dp[(1 << N) - 1][i] + dist[dest[i]][b]);
    }
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
