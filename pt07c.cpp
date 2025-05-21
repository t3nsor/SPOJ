// 2025-05-20
// The weighted version of the minimum diameter spanning tree problem is much
// harder than the unweighted version (see MDST) because we can no longer just
// try each vertex and the midpoint of each edge as candidate locations for the
// absolute 1-center. The algorithm to find the absolute 1-center is described
// here: https://stackoverflow.com/a/41209943/481267
//
// For dense graphs, it's possible to implement this algorithm in O(VE). The
// approach is described in the official solution. After finding the distance
// matrix, we prepare, for each vertex, a sorted list of vertices by their
// distance from the former. When finding the local centre for a given edge
// (u, v), we use the list of vertices sorted by their distance from `u` to
// prepare a sublist of vertices sorted by their distance from `u` consisting of
// all "maximal" vertices: a vertex `i` is not maximal if there exists some
// other vertex `j` such that dist[u][i] <= dist[u][j] and dist[v][i] <=
// dist[v][j] also. The local centre is either at one of the endpoints of the
// edge or at an intersection point between the curves of two adjacent vertices
// in the maximal sublist. (This follows from the fact that when each such curve
// ceases to be on the upper envelope, it is necessarily replaced by the one
// immediately preceding it in the list since it can't intersect any other curve
// without intersecting that one first.)
//
// This version is much faster than the O(VE log V) version (see commit
// e90fa6e).
#include <algorithm>
#include <iostream>
#include <set>
#include <utility>
#include <vector>
using namespace std;
pair<int, int> find_local_center(int u, const vector<int>& order, int v, int l,
                                 const vector<vector<int>>& dist) {
    const int V = dist.size();
    vector<int> hull;
    for (const int i : order) {
        if (!hull.empty() &&
            dist[u][i] == dist[u][hull.back()] &&
            dist[v][i] <= dist[v][hull.back()]) continue;
        while (!hull.empty() && dist[v][i] >= dist[v][hull.back()]) {
            hull.pop_back();
        }
        hull.push_back(i);
    }
    pair<int, int> result(l, dist[v][hull.front()]);
    if (result.second > dist[u][hull.back()]) {
        result.first = 0;
        result.second = dist[u][hull.back()];
    }
    for (int i = 0; i + 1 < hull.size(); i++) {
        // peak point for `hull[i + 1]`
        const int p = max(0, min(l, (dist[v][hull[i + 1]]
                                     - dist[u][hull[i + 1]]
                                     + l) / 2));
        // value for `hull[i + 1]` at peak point
        const int val = min(p + dist[u][hull[i + 1]],
                            l - p + dist[v][hull[i + 1]]);
        // intersection point
        const int x = (val + p - dist[u][hull[i]]) / 2;
        const int minval = min(x + dist[u][hull[i]],
                               l - x + dist[v][hull[i]]);
        if (minval < result.second) {
            result.first = x;
            result.second = minval;
        }
    }
    return result;
}
int main() {
    ios::sync_with_stdio(false);
    int V, E; cin >> V >> E;
    vector<vector<int>> dist(V, vector<int>(V, 1e9));
    vector<vector<int>> edge(V, vector<int>(V, 1e9));
    for (int i = 0; i < E; i++) {
        int u, v, w; cin >> u >> v >> w; u--; v--;
        // double all edge lengths to avoid fractions
        edge[u][v] = edge[v][u] = min(edge[u][v], 2*w);
        dist[u][v] = dist[v][u] = edge[u][v];
    }
    if (V <= 1) {
        cout << "0\n";
        return 0;
    }
    for (int i = 0; i < V; i++) {
        dist[i][i] = 0;
    }
    // find all pairs shortest paths
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
    // prepare sorted lists of vertices
    vector<vector<int>> order;
    for (int i = 0; i < V; i++) {
        vector<int> v;
        for (int j = 0; j < V; j++) {
            v.push_back(j);
        }
        sort(v.begin(), v.end(),
             [&](int u, int v) { return dist[i][u] < dist[i][v]; });
        order.push_back(move(v));
    }
    // find absolute 1-center
    pair<int, int> bestedge;
    int bestpos;
    int bestr = 1e9;
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (edge[i][j] >= 1e9) continue;
            const auto lc = find_local_center(i, order[i], j, edge[i][j], dist);
            if (lc.second < bestr) {
                bestedge = make_pair(i, j);
                bestpos = lc.first;
                bestr = lc.second;
            }
        }
    }
    // find shortest path tree
    vector<pair<int, int>> dist2(V, make_pair(1e9, -1));
    vector<int> active(V, 0);
    dist2[bestedge.first].first = bestpos;
    dist2[bestedge.first].second = -1;
    active[bestedge.first] = 1;
    dist2[bestedge.second].first =
      edge[bestedge.first][bestedge.second] - bestpos;
    dist2[bestedge.second].second = -1;
    active[bestedge.second] = 1;
    for (;;) {
        int u = -1;
        for (int i = 0; i < V; i++) {
            if (active[i] &&
                (u < 0 || dist2[i].first < dist2[u].first)) {
                u = i;
            }
        }
        if (u == -1) break;
        active[u] = 0;
        for (int v = 0; v < V; v++) {
            if (edge[u][v] >= 1e9) continue;
            if (edge[u][v] + dist2[u].first < dist2[v].first) {
                dist2[v].first = edge[u][v] + dist2[u].first;
                dist2[v].second = u;
                active[v] = 1;
            }
        }
    }
    // diameter with original weights = radius with doubled weights
    cout << bestr << '\n';
    int cnt = 0;
    for (int i = 0; i < V; i++) {
        if (dist2[i].second >= 0) {
            int u = i;
            int v = dist2[i].second;
            if (u > v) swap(u, v);
            cout << u + 1 << ' ' << v + 1 << '\n';
            ++cnt;
        }
    }
    if (cnt < V - 1) {
        cout << bestedge.first + 1 << ' ' << bestedge.second + 1 << '\n';
    }
}
