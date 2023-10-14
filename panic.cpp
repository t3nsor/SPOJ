// 2023-10-14
// The idea behind this problem is pretty simple, but the implementation is
// lengthy. First, we find a list of places that the panic will take the longest
// to reach. This can be done using Dijkstra's, but we have to keep in mind that
// it can also be somewhere along an edge---so we need to do some math to figure
// out exactly where along the edge it happens. Then, we need to determine where
// to start such that we can make it to one of the best final spots. A node
// satisfies this criterion if the shortest distance from the starting points of
// the panic plus the shortest distance to a best final spot equals the total
// time it takes for the panic to spread to the final spots.
#include <limits.h>
#include <queue>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
struct Edge {
    int other;
    int wt;
    int rev;
};
struct Mixed {
    // represents i + p/q (p >= 0, q > 0)
    int i;
    int p;
    int q;
};
bool operator<(const Mixed& m1, const Mixed& m2) {
    return m1.i < m2.i ||
           (m1.i == m2.i && m1.p * m2.q < m2.p * m1.q);
}
bool operator==(const Mixed& m1, const Mixed& m2) {
    return m1.i == m2.i && m1.p * m2.q == m2.p * m1.q;
}
bool operator>(const Mixed& m1, const Mixed& m2) {
    return m2 < m1;
}
Mixed divide(int x, int y) {
    return Mixed{x / y, x % y, y};
}
void do_testcase() {
    int V, E, nbombs; scanf("%d %d %d", &V, &E, &nbombs);
    vector<vector<Edge>> adj(V + 1);
    while (E--) {
        int u, v, w1, w2; scanf("%d %d %d %d", &u, &v, &w1, &w2);
        adj[u].push_back(Edge{v, w1, (int)adj[v].size()});
        adj[v].push_back(Edge{u, w2, (int)adj[u].size() - 1});
    }
    while (nbombs--) {
        int bomb; scanf("%d", &bomb);
        adj[0].push_back(Edge{bomb, 0, -1});
    }
    vector<int> dist(V + 1, INT_MAX);
    dist[0] = 0;
    vector<int> vis(V + 1, 0);
    priority_queue<pair<int, int>,
                   vector<pair<int, int>>,
                   greater<pair<int, int>>> Q;
    Q.emplace(0, 0);
    Mixed best_time = {-1, 0, 1};
    vector<pair<int, int>> best_places;
    while (!Q.empty()) {
        const auto p = Q.top();
        Q.pop();
        const int d = p.first;
        const int u = p.second;
        if (vis[u]) continue;
        vis[u] = true;
        // check if this vertex is a longest-surviving place
        if (Mixed{dist[u], 0, 1} > best_time) {
            best_time = Mixed{dist[u], 0, 1};
            best_places.clear();
            best_places.emplace_back(u, -1);
        } else if (Mixed{dist[u], 0, 1} == best_time) {
            best_places.emplace_back(u, -1);
        }
        for (int i = 0; i < adj[u].size(); i++) {
            const auto& e = adj[u][i];
            if (vis[e.other]) {
                // Check if somewhere inside this edge is a longest-surviving
                // place. Note that `u` is further than `e.other`. So first we
                // have to check how long it takes for them to get here.
                const int rwt = adj[e.other][e.rev].wt;
                if (dist[e.other] + rwt == dist[u]) {
                    // No one runs from `u` to `e.other`.
                    continue;
                }
                const int diff = dist[u] - dist[e.other];
                Mixed t = divide(e.wt * rwt + rwt * diff, e.wt + rwt);
                t.i += dist[e.other];
                if (t > best_time) {
                    best_time = t;
                    best_places.clear();
                    best_places.emplace_back(u, i);
                } else if (t == best_time) {
                    best_places.emplace_back(u, i);
                }
            } else if (d + e.wt < dist[e.other]) {
                dist[e.other] = d + e.wt;
                Q.emplace(d + e.wt, e.other);
            }
        }
    }
    // check for unreachable nodes
    vector<int> unreachable;
    for (int i = 1; i <= V; i++) {
        if (!vis[i]) unreachable.push_back(i);
    }
    if (!unreachable.empty()) {
        printf("%d", unreachable[0]);
        for (int i = 1; i < unreachable.size(); i++) {
            printf(" %d", unreachable[i]);
        }
        putchar('\n');
        return;
    }
    // We have a list of best places; now we have to do a second Dijkstra's from
    // them. In the case of a point that is along an edge, we don't actually
    // want to have to introduce a new node for such a point, so we just take
    // the endpoints of the edge as possible starting points.
    vector<Mixed> dist2(V + 1, Mixed{INT_MAX, 0, 1});
    vector<int> vis2(V + 1, 0);
    for (const auto place : best_places) {
        const int far = place.first;
        const int i = place.second;
        if (i < 0) {
            dist2[far] = Mixed{0, 0, 1};
        } else {
            const int near = adj[far][i].other;
            const int diff = dist[far] - dist[near];
            const int w = adj[far][i].wt;
            const int r = adj[near][adj[far][i].rev].wt;
            const Mixed tf = divide(w*(r + dist[near] - dist[far]), w + r);
            const Mixed tn = divide(r*(w + dist[far] - dist[near]), w + r);
            dist2[far] = min(dist2[far], tf);
            dist2[near] = min(dist2[near], tn);
        }
    }
    priority_queue<pair<Mixed, int>,
                   vector<pair<Mixed, int>>,
                   greater<pair<Mixed, int>>> Q2;
    for (int i = 1; i <= V; i++) {
        if (dist2[i].i < INT_MAX) {
            Q2.emplace(dist2[i], i);
        }
    }
    while (!Q2.empty()) {
        const auto p = Q2.top();
        Q2.pop();
        const Mixed& d = p.first;
        const int u = p.second;
        if (vis2[u]) continue;
        vis2[u] = true;
        for (int i = 0; i < adj[u].size(); i++) {
            // use reverse edges
            const auto& e = adj[u][i];
            if (vis2[e.other]) continue;
            const int w = adj[e.other][e.rev].wt;
            Mixed new_dist = d;
            new_dist.i += w;
            if (new_dist < dist2[e.other]) {
                dist2[e.other] = new_dist;
                Q2.emplace(new_dist, e.other);
            }
        }
    }
    vector<int> result;
    for (int i = 1; i <= V; i++) {
        dist2[i].i += dist[i];
        if (dist2[i] == best_time) result.push_back(i);
    }
    printf("%d", result[0]);
    for (int i = 1; i < result.size(); i++) {
        printf(" %d", result[i]);
    }
    putchar('\n');
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
