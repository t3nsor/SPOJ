// 2025-06-06
// In the official data, c <= 199 and there is at most one edge incident upon a
// given pair of nodes.
#include <algorithm>
#include <iostream>
#include <limits.h>
#include <stack>
#include <vector>
using namespace std;
struct Edge { int a; int b; int c; };
void do_testcase(int cs, int V, int E) {
    vector<Edge> edge;
    for (int i = 0; i < E; i++) {
        int u, v, c; cin >> u >> v >> c;
        edge.push_back({u - 1, v - 1, c});
    }
    int curadj[15][15];
    int best = INT_MAX;
    stack<int> S;
    for (int i = 1; i < (1 << E); i++) {
        int adj[15][15] = {0};
        int cost = 0;
        for (int j = 0; j < E; j++) {
            if (i & (1 << j)) {
                adj[edge[j].a][edge[j].b] = adj[edge[j].b][edge[j].a] = 1;
                cost += edge[j].c;
            }
        }
        if (cost >= best) continue;
        bool reli = true;
        for (int j = 0; j < E; j++) {
            if (!(i & (1 << j))) continue;
            adj[edge[j].a][edge[j].b] = adj[edge[j].b][edge[j].a] = 0;
            int vis[15] = {0};
            S.push(0);
            while (!S.empty()) {
                const int u = S.top();
                S.pop();
                if (vis[u]) continue;
                vis[u] = 1;
                for (int v = 0; v < V; v++) if (adj[u][v]) S.push(v);
            }
            adj[edge[j].a][edge[j].b] = adj[edge[j].b][edge[j].a] = 1;
            if (find(vis, vis + V, 0) != vis + V) {
                reli = false;
                break;
            }
        }
        if (reli) best = cost;
    }
    if (best <= 1e4) {
        cout << "The minimal cost for test case " << cs << " is " << best;
    } else {
        cout << "There is no reliable net possible for test case " << cs;
    }
    cout << ".\n";
}
int main() {
    for (int cs = 1;; ++cs) {
        int V, E; cin >> V >> E;
        if (V == 0) break;
        do_testcase(cs, V, E);
    }
}
