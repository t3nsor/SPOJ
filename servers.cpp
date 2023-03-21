// 2023-03-20
// Basically all we have to do is sort the nodes in decreasing order of rank and
// run Dijkstra's with each node as the source in turn. When the source `s` is a
// node of rank `r`, then once we reach a node `u` that already has a
// greater-rank node `w` at an equal or lesser distance, we don't explore
// further from that node---because doing so can only yield paths that will
// also give a distance that is less than or equal to the distance from `w`
// (since paths starting from `w` can use those additional edges as well). Every
// time we reach a node that *doesn't* have this property, we know that `s` is
// in its interesting set so we increment the total count. The total number of
// times we explore outward is therefore bounded by the final result, which is
// no more than 30n.
//
// To reduce the constant factor (since an earlier version didn't get accepted)
// we do the standard Dijkstra's optimization of updating distances before
// visiting nodes in order to reduce the number of entries we have to push onto
// the heap. Another constant factor optimization specific to this problem is
// to use the fact that maximum-rank nodes are interesting to all nodes, so we
// don't actually have to run Dijkstra's individually from each such node; we
// can do a single Dijkstra's using all such nodes as source nodes, and only
// visit each node once, at which point the minimum distance to any
// maximum-rank node will be recorded.
#include <algorithm>
#include <cstdio>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;
void multi_dijkstra(const vector<int>& sources,
                    const vector<vector<pair<int, int>>>& adjlist,
                    vector<int>& dist) {
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>> Q;
    vector<int> vis(dist.size(), false);
    for (int i = 0; i < sources.size(); i++) {
        Q.emplace(0, sources[i]);
        dist[sources[i]] = 0;
    }
    while (!Q.empty()) {
        const auto top = Q.top();
        Q.pop();
        const int u = top.second;
        const int d = top.first;
        if (vis[u]) continue;
        vis[u] = true;
        for (const auto& edge : adjlist[u]) {
            const int v = edge.second;
            const int weight = edge.first;
            if (d + weight >= dist[v]) continue;
            dist[v] = d + weight;
            Q.emplace(d + weight, v);
        }

    }
}
void dijkstra(const int src, const vector<vector<pair<int, int>>>& adjlist,
              const vector<int>& prev_dist, unordered_map<int, int>& rank_dist,
              int& result) {
    // We use 3 `dist` arrays:
    // * `prev_dist[i]` is the distance from `i` to the closest node whose rank
    //   is strictly greater than that of `src`
    // * `rank_dist[i]` is the distance from `i` to the closest node whose rank
    //   equals that of `src`; we use this to update `prev_dist` once all nodes
    //   of this rank have been used as sources
    // * `local_dist` is the distance array for this iteration of Dijkstra's.
    // To avoid using up linear time on each iteration of Dijkstra's, we use
    // hash maps instead of vectors.
    unordered_map<int, int> local_dist;
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>> Q;
    local_dist[src] = 0;
    Q.emplace(0, src);
    ++result;
    while (!Q.empty()) {
        const auto top = Q.top();
        Q.pop();
        const int u = top.second;
        const int d = top.first;
        if (local_dist[u] < d) continue;
        local_dist[u] = d - 1;  // avoid revisiting
        const auto it = rank_dist.find(u);
        if (it == rank_dist.end()) {
            rank_dist[u] = d;
        } else {
            it->second = min(it->second, d);
        }
        for (const auto& edge : adjlist[u]) {
            const int v = edge.second;
            const int weight = edge.first;
            if (d + weight >= prev_dist[v]) continue;
            const auto it = local_dist.find(v);
            if (it == local_dist.end()) {
                local_dist[v] = d + weight;
                result++;
            } else if (it->second <= d + weight) {
                continue;
            } else {
                it->second = d + weight;
            }
            Q.emplace(d + weight, v);
        }
    }
}
void do_testcase() {
    int V, E; scanf("%d %d", &V, &E);
    vector<vector<pair<int, int>>> adjlist(V);
    vector<int> rank(V);
    vector<int> by_rank[11];
    int max_rank = 0;
    int max_rank_cnt = 0;
    for (int i = 0; i < V; i++) {
        scanf("%d", &rank[i]);
        if (rank[i] > max_rank) {
            max_rank = rank[i];
            max_rank_cnt = 0;
        }
        if (rank[i] == max_rank) {
            max_rank_cnt++;
        }
        by_rank[rank[i]].push_back(i);
    }
    for (int i = 0; i < E; i++) {
        int a, b, t; scanf("%d %d %d", &a, &b, &t); a--; b--;
        adjlist[a].emplace_back(t, b);
        adjlist[b].emplace_back(t, a);
    }
    int result = V * max_rank_cnt;
    vector<int> prev_dist(V, 1e9);
    multi_dijkstra(by_rank[max_rank], adjlist, prev_dist);
    for (int r = max_rank - 1; r >= 1; r--) {
        unordered_map<int, int> rank_dist;
        for (const int src : by_rank[r]) {
            dijkstra(src, adjlist, prev_dist, rank_dist, result);
        }
        for (const auto& p : rank_dist) {
            prev_dist[p.first] = min(prev_dist[p.first], p.second);
        }
    }
    printf("%d\n", result);
}
int main() {
    int z; scanf("%d", &z);
    while (z--) do_testcase();
}
