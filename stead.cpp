// 2024-08-31
// We can try all possible ranges of rankings starting from smallest to largest
// and stop when we find a range such that every cow can be placed in some barn
// that falls within that range of its ranklist, and no barn's capacity is
// exceeded. To determine when this is possible, we use max flow, where there's
// an edge of capacity 1 from the source to each cow, an edge from each barn to
// the sink with capacity equal to the barn's capacity, and an edge from each
// cow to each barn that is within range on its ranklist. If the max flow in
// this network is equal to the number of cows, it corresponds to an assignment
// of each cow to a barn that doesn't exceed any barn's capacity.
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
// push-relabel implementation taken from fastflow.cpp
struct st {
    int v;
    int cap;
    int back;
};
long long get_max_flow(vector<vector<st>>&& adj, int s, int t) {
    const int V = adj.size();
    vector<long long> excess(V, 0);
    vector<int> active(V, 0);
    vector<int> height(V, 0);
    vector<int> hcnt(2*V + 1, 0);
    queue<int> Q;
    height[s] = V;
    hcnt[V] = 1;
    hcnt[0] = V - 1;
    for (int i = 0; i < V; i++) {
        if (i == s || i == t) continue;
        active[i] = 1;
        Q.push(i);
    }
    for (auto& edge : adj[s]) {
        excess[edge.v] += edge.cap;
        adj[edge.v][edge.back].cap = edge.cap;
        edge.cap = 0;
    }
    while (!Q.empty()) {
        const int u = Q.front();
        Q.pop();
        active[u] = 0;
        int best = 1e9;
        for (auto& edge : adj[u]) {
            if (!edge.cap) continue;
            if (height[u] == height[edge.v] + 1) {
                const int x = min<long long>(excess[u], edge.cap);
                excess[u] -= x;
                excess[edge.v] += x;
                edge.cap -= x;
                adj[edge.v][edge.back].cap += x;
                if (edge.v != s && edge.v != t && !active[edge.v]) {
                    active[edge.v] = 1;
                    Q.push(edge.v);
                }
                if (excess[u] == 0) break;
            } else {
                best = min(best, height[edge.v]);
            }
        }
        if (excess[u] > 0) {
            const int oldh = height[u];
            height[u] = best + 1;
            hcnt[best + 1]++;
            if (0 == --hcnt[oldh] && oldh < V) {
                for (int i = 0; i < V; i++) {
                    if (i != s && i != t &&
                        height[i] > oldh && height[i] <= V) {
                        hcnt[height[i]]--;
                        height[i] = V + 1;
                    }
                }
            }
            if (!active[u]) {
                active[u] = 1;
                Q.push(u);
            }
        }
    }
    return excess[t];
}
int main() {
    ios::sync_with_stdio(false);
    int N, B; cin >> N >> B;
    vector<vector<int>> ranking(N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < B; j++) {
            int barn; cin >> barn; ranking[i].push_back(barn - 1);
        }
    }
    vector<int> barnsize(B);
    for (int i = 0; i < B; i++) cin >> barnsize[i];
    for (int sz = 1; sz < B; sz++) {
        for (int m = 0; m + sz <= B; m++) {
            const int M = m + sz - 1;
            vector<vector<st>> graph(2 + N + B);
            for (int barn = 0; barn < B; barn++) {
                graph[1 + N + barn].push_back(
                  {1 + N + B, barnsize[barn], barn});
                graph[1 + N + B].push_back({1 + N + barn, 0, 0});
            }
            for (int cow = 0; cow < N; cow++) {
                graph[0].push_back({cow + 1, 1, 0});
                graph[cow + 1].push_back({0, 0, cow});
                for (int i = m; i <= M; i++) {
                    const int barn = ranking[cow][i];
                    graph[cow + 1].push_back(
                      {1 + N + barn, 1, int(graph[1 + N + barn].size())});
                    graph[1 + N + barn].push_back(
                      {cow + 1, 0, int(graph[cow + 1].size() - 1)});
                }
            }
            if (get_max_flow(move(graph), 0, 1 + N + B) == N) {
                cout << sz << '\n';
                return 0;
            }
        }
    }
    cout << B << '\n';
}
