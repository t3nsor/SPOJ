// 2024-02-18
// This problem can be formulated as a mincost maxflow. Since we want to force
// each of N vertices to be used exactly once, we should split each vertex into
// an in-vertex and an out-vertex, where there's an edge from the source to each
// out-vertex with capacity 1 and an edge from each in-vertex to the sink with
// capacity 1. These edges have cost 0 while each original edge in the graph has
// capacity 1 and cost equal to its weight. When this is done, we see that we
// actually have the special case of weighted bipartite matching.

#include <algorithm>
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;

// Stanford's implementation of weighted bipartite matching, modified
// (originally for SCITIES.cpp) to use integer weights and discard matching
// information. There is no known license for this code, so copy it at your own
// risk.

typedef vector<int> VI;
typedef vector<VI> VVI;

int MinCostMatching(const VVI &cost) {
    int n = cost.size();

    // construct dual feasible solution
    VI u(n);
    VI v(n);
    for (int i = 0; i < n; i++) {
        u[i] = cost[i][0];
        for (int j = 1; j < n; j++) u[i] = min(u[i], cost[i][j]);
    }
    for (int j = 0; j < n; j++) {
        v[j] = cost[0][j] - u[0];
        for (int i = 1; i < n; i++) v[j] = min(v[j], cost[i][j] - u[i]);
    }

    // construct primal solution satisfying complementary slackness
    VI Lmate(n, -1);
    VI Rmate(n, -1);
    int mated = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (Rmate[j] != -1) continue;
            if (cost[i][j] - u[i] - v[j] == 0) {
                Lmate[i] = j;
                Rmate[j] = i;
                mated++;
                break;
            }
        }
    }

    VI dist(n);
    VI dad(n);
    VI seen(n);

    // repeat until primal solution is feasible
    while (mated < n) {

        // find an unmatched left node
        int s = 0;
        while (Lmate[s] != -1) s++;

        // initialize Dijkstra
        fill(dad.begin(), dad.end(), -1);
        fill(seen.begin(), seen.end(), 0);
        for (int k = 0; k < n; k++) 
            dist[k] = cost[s][k] - u[s] - v[k];

        int j = 0;
        while (true) {

            // find closest
            j = -1;
            for (int k = 0; k < n; k++) {
                if (seen[k]) continue;
                if (j == -1 || dist[k] < dist[j]) j = k;
            }
            seen[j] = 1;

            // termination condition
            if (Rmate[j] == -1) break;

            // relax neighbors
            const int i = Rmate[j];
            for (int k = 0; k < n; k++) {
                if (seen[k]) continue;
                const int new_dist = dist[j] + cost[i][k] - u[i] - v[k];
                if (dist[k] > new_dist) {
                    dist[k] = new_dist;
                    dad[k] = j;
                }
            }
        }

        // update dual variables
        for (int k = 0; k < n; k++) {
            if (k == j || !seen[k]) continue;
            const int i = Rmate[k];
            v[k] += dist[k] - dist[j];
            u[i] -= dist[k] - dist[j];
        }
        u[s] += dist[j];

        // augment along path
        while (dad[j] >= 0) {
            const int d = dad[j];
            Rmate[j] = Rmate[d];
            Lmate[Rmate[j]] = j;
            j = d;
        }
        Rmate[j] = s;
        Lmate[s] = j;

        mated++;
    }

    int value = 0;
    for (int i = 0; i < n; i++)
        value += cost[i][Lmate[i]];

    return value;
}

// non-prewritten code starts here
void do_testcase() {
    int V, E; cin >> V >> E;
    vector<vector<int>> wt(2*V, vector<int>(2*V, 1e6));
    for (int i = 0; i < V; i++) {
        wt[2*i][2*i + 1] = 0;
    }
    for (int i = 0; i < E; i++) {
        int v1, v2, w; cin >> v1 >> v2 >> w; --v1; --v2;
        wt[2*v1 + 1][2*v2] = w;
    }
    cout << MinCostMatching(wt) << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
