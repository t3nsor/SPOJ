// 2025-06-10
#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;
void do_testcase() {
    int V, E; scanf("%d %d", &V, &E);
    int adj[100][100];
    memset(adj, 0x01, sizeof(adj));
    while (E--) {
        int a, b, c; scanf("%d %d %d", &a, &b, &c); --a; --b;
        adj[a][b] = adj[b][a] = c;
    }
    long long paths[100][100][100];  // s, t, v
    memset(paths, 0, sizeof(paths));
    for (int s = 0; s < V; s++) {
        // run Dijkstra's from `s`
        vector<int> dist(V, 1e6);
        vector<int> active(V, 0);
        dist[s] = 0;
        active[s] = 1;
        paths[s][s][s] = 1;
        for (;;) {
            // find next node to visit (break ties arbitrarily)
            int v = -1, d = 1e6;
            for (int i = 0; i < V; i++) {
                if (active[i] && (v == -1 || dist[i] < dist[v])) v = i;
            }
            if (v < 0) break;
            active[v] = 0;
            if (v != s) {
                // calculate `paths[s][v]` by looking through all possible
                // predecessors
                for (int u = 0; u < V; u++) {
                    if (dist[u] + adj[u][v] == dist[v]) {
                        for (int i = 0; i < V; i++) {
                            paths[s][v][i] += paths[s][u][i];
                        }
                        paths[s][v][v] += paths[s][u][u];
                    }
                }
            }
            for (int w = 0; w < V; w++) {
                if (adj[v][w] <= 1000 &&
                    dist[w] > dist[v] + adj[v][w]) {
                    dist[w] = dist[v] + adj[v][w];
                    active[w] = true;
                }
            }
        }
    }
    for (int i = 0; i < V; i++) {
        double imp = 0;
        for (int s = 0; s < V; s++) {
            if (s == i) continue;
            for (int t = 0; t < V; t++) {
                if (t == i) continue;
                imp += paths[s][t][i] / double(paths[s][t][t]);
            }
        }
        printf("%.3f\n", imp);
    }
}
int main() {
    int T; scanf("%d", &T);
    while (T--) do_testcase();
}
