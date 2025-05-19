// 2025-05-19
// The weighted version of the minimum diameter spanning tree problem is much
// harder than the unweighted version (see MDST) because we can no longer just
// try each vertex and the midpoint of each edge as candidate locations for the
// absolute 1-center. The algorithm to find the absolute 1-center is described
// here: https://stackoverflow.com/a/41209943/481267
// Note that for dense graphs the running time will be O(n^3 log n). The use of
// ordered sets, although quite natural, makes the constant factor large. This
// is the slowest accepted solution on SPJO.
#include <algorithm>
#include <iostream>
#include <set>
#include <utility>
#include <vector>
using namespace std;
pair<int, int> find_local_center(int u, int v, int l,
                                 const vector<vector<int>>& dist) {
    // We maintain two ordered sets of vertices: those for whom the distance to
    // the further of `u` and `v` is currently increasing (with a slope of 1),
    // and those for whom it's decreasing (with a slope of -1). These sets are
    // called `pos` and `neg`, respectively. For each vertex we also compute the
    // position `p` along the edge at which the vertex should be removed from
    // `pos` and inserted into `neg`.
    const int V = dist.size();
    vector<int> intercept(V);
    set<pair<int, int>> pos, neg;
    vector<pair<int, int>> event;
    for (int i = 0; i < V; i++) {
        const int p = max(0, min(l, (-dist[u][i] + dist[v][i] + l) / 2));
        intercept[i] = dist[u][i];
        pos.emplace(intercept[i], i);
        event.emplace_back(p, i);
    }
    pair<int, int> result(-1, 1e9);
    event.emplace_back(0, -1);
    event.emplace_back(l, -1);
    sort(event.begin(), event.end());
    int last = 0;
    for (const auto e : event) {
        const int cur = e.first;
        int pos_hi = -1;
        int neg_hi = -1;
        if (!pos.empty()) {
            pos_hi = prev(pos.end())->second;
        }
        if (!neg.empty()) {
            neg_hi = prev(neg.end())->second;
        }
        int lowpos = cur;
        if (pos_hi >= 0 && neg_hi >= 0) {
            // If the highest positive line and the highest negative line
            // intersect somewhere in the middle of the current interval, that
            // intersection point is better than the endpoints
            const int x = (intercept[neg_hi] - intercept[pos_hi]) / 2;
            if (x > last && x < cur) lowpos = x;
        }
        int top = -1;
        if (pos_hi >= 0) top = max(top, intercept[pos_hi] + lowpos);
        if (neg_hi >= 0) top = max(top, intercept[neg_hi] - lowpos);
        if (top < result.second) {
            result.first = lowpos;
            result.second = top;
        }
        if (e.second >= 0) {
            pos.erase(make_pair(intercept[e.second], e.second));
            const int val = min(dist[u][e.second] + cur,
                                dist[v][e.second] + l - cur);
            intercept[e.second] = val + cur;
            neg.emplace(intercept[e.second], e.second);
        }
        last = cur;
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
    // find absolute 1-center
    pair<int, int> bestedge;
    int bestpos;
    int bestr = 1e9;
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (edge[i][j] >= 1e9) continue;
            const auto lc = find_local_center(i, j, edge[i][j], dist);
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
