// 2023-10-18
// The problem description really buries the lede: the maximum number of
// directed edges is 4000, because each vertex has a degree between 0 and 4.
// This suggests that quadratic time in the number of edges is acceptable (and
// the time limit of 17s further confirms this). Thus, to find the shortest
// cycle, we can use the following simple algorithm: for each edge in the graph,
// remove the edge and then find the shortest path between the endpoints using
// Dijsktra's algorithm. The overall running time is then O(E^2 log V). To count
// the number of such shortest cycles, we use a simple modification to
// Dijkstra's algorithm that counts the number of shortest paths, then we sum
// over all cycles. But to avoid multiply counting a cycle (i.e., once for each
// edge) we assign each edge a numerical ID and count a cycle only when the
// removed edge has the smallest ID in the cycle (which we can do simply by
// ignoring all edges with smaller IDs while doing Dijkstra's).
#include <iostream>
#include <limits.h>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
struct Edge {
    int dest;
    int wt;
    int id;
};
void do_testcase() {
    int V, E; cin >> V >> E;
    vector<vector<Edge>> adj(V);
    vector<char> used(E);
    for (int i = 0; i < E; i++) {
        int u, v, d; cin >> u >> v >> d; u--; v--;
        adj[u].push_back(Edge{v, d, i});
        adj[v].push_back(Edge{u, d, i});
    }
    int shortest = INT_MAX;
    int nshortest = 0;
    for (int i = 0; i < V; i++) {
        for (const Edge& e : adj[i]) {
            const int id = e.id;
            if (used[id]) continue;
            used[id] = true;
            vector<int> dist(V, INT_MAX);
            vector<int> cnt(V, 0);
            vector<char> visited(V, 0);
            priority_queue<pair<int, int>,
                           vector<pair<int, int>>,
                           greater<pair<int, int>>> Q;
            dist[i] = 0;
            cnt[i] = 1;
            Q.emplace(0, i);
            while (!Q.empty()) {
                const auto p = Q.top();
                Q.pop();
                if (visited[p.second]) continue;
                visited[p.second] = true;
                if (p.second == e.dest) break;
                for (const Edge& e2 : adj[p.second]) {
                    if (e2.id <= id) continue;
                    if (e2.wt + dist[p.second] < dist[e2.dest]) {
                        dist[e2.dest] = e2.wt + dist[p.second];
                        cnt[e2.dest] = cnt[p.second];
                        Q.emplace(dist[e2.dest], e2.dest);
                    } else if (e2.wt + dist[p.second] == dist[e2.dest]) {
                        cnt[e2.dest] += cnt[p.second];
                    }
                }
            }
            if (dist[e.dest] == INT_MAX) continue;
            dist[e.dest] += e.wt;
            if (dist[e.dest] < shortest) {
                shortest = dist[e.dest];
                nshortest = cnt[e.dest];
            } else if (dist[e.dest] == shortest) {
                nshortest += cnt[e.dest];
            }
        }
    }
    if (shortest == INT_MAX) {
        cout << "0 0\n";
    } else {
        cout << shortest << ' ' << nshortest << '\n';
    }
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
