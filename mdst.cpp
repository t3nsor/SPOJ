// 2023-11-20
// To find a minimum diameter spanning tree, we can find any "absolute 1-center"
// of the graph (it may be on an edge) and then compute any shortest paths tree
// from the absolute 1-center. However, in this case since we only need to find
// the diameter of the MDST, that's just 2 times the distance of the farthest
// node from the absolute 1-center.
#include <algorithm>
#include <stdio.h>
#include <queue>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;
void do_testcase() {
    int n; scanf("%d", &n);
    vector<vector<int>> adj(n);
    for (int i = 0; i < n; i++) {
        int u; scanf("%d", &u); --u;
        int m; scanf("%d", &m);
        while (m--) {
            int v; scanf("%d", &v); --v;
            adj[u].push_back(v);
        }
    }
    // compute all pairs shortest paths; also find center
    vector<vector<int>> dist(n, vector<int>(n, -1));
    pair<int, int> bestc;
    int bestr = 10000;
    for (int i = 0; i < n; i++) {
        queue<int> Q;
        Q.push(i);
        dist[i][i] = 0;
        while (!Q.empty()) {
            const auto u = Q.front();
            Q.pop();
            for (const auto v : adj[u]) {
                if (dist[i][v] == -1) {
                    dist[i][v] = dist[i][u] + 1;
                    Q.push(v);
                }
            }
        }
        int r = -1;
        for (int j = 0; j < n; j++) {
            r = max(r, dist[i][j]);
        }
        if (2 * r < bestr) {
            bestr = 2 * r;
            bestc = {i, -1};
        }
    }
    // check whether the actual center is on an edge
    for (int i = 0; i < n; i++) {
        for (const auto j : adj[i]) {
            if (j < i) continue;
            int r = -1;
            for (int k = 0; k < n; k++) {
                int d = 1 + 2 * min(dist[i][k], dist[j][k]);
                r = max(r, d);
            }
            if (r < bestr) {
                bestr = r;
                bestc = {i, j};
            }
        }
    }
    printf("%d\n", bestr);
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
