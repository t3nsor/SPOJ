// 2026-04-27
// This is a max flow problem.  The official solution also uses max flow but is
// a bit different (https://ipsc.ksp.sk/2002/real/solutions/f.html).
//
// We first make team N win all matches that involve team N, then for every
// other match, we initially arbitrarily assign both points to the "left" side
// of the match (i.e. the team that's given first on the corresponding input
// line).  Then:
// * Introduce an in-vertex and out-vertex for each team other than team N.
// * For every team t < N that now has at least as many points as team N, let
//   d(t) be the number of points we have to take away from it so that it will
//   have fewer points than team N.  Add an edge from the source to the
//   in-vertex for t, with capacity d(t).
// * For every team t that now has at least 2 fewer points than team N, let d(t)
//   be the maximum number of points we can give it so that it will still have
//   fewer than team N.  Add an edge from the in-vertex to the out-vertex for t,
//   with capacity d(t).
// * Add an infinite-capacity edge from each out-vertex to the sink.
// * For each match that doesn't involve team N, add an edge of capacity 2 from
//   the left team's in-vertex to the right team's in-vertex.
// A flow represents a distribution of points away from the teams that have too
// many points, without giving too many points to any other teams.  If the max
// flow equals the total capacity of edges from the source, we successfully
// distributed away all the excess points initially held.
//
// The max flow implementation was taken from fastflow.cpp.
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
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
bool do_testcase(int N) {
    int M; cin >> M;
    vector<int> pts(N);
    int maxpts = 0;
    for (int i = 0; i < N; i++) {
        cin >> pts[i];
        if (i < N - 1) maxpts = max(maxpts, pts[i]);
    }
    if (maxpts - pts[N - 1] >= 2*M) return false;
    if (pts[N - 1] - maxpts > 2*M) return true;
    vector<vector<st>> adj(2*N);  // source 2N - 2, sink 2N - 1
    for (int i = 0; i < M; i++) {
        int x, y; cin >> x >> y; --x; --y;
        if (x == N - 1 || y == N - 1) {
            pts[N - 1] += 2;
        } else {
            pts[x] += 2;
            adj[2*x].push_back(st{2*y, 2, (int)adj[2*y].size()});
            adj[2*y].push_back(st{2*x, 0, (int)adj[2*x].size()-1});
        }
    }
    int need = 0;
    for (int i = 0; i < N - 1; i++) {
        if (pts[i] >= pts[N - 1]) {
            const int diff = pts[i] - pts[N - 1] + 1;
            need += diff;
            adj[2*N-2].push_back(st{2*i, diff, (int)adj[2*i].size()});
            adj[2*i].push_back(st{2*N-2, 0, (int)adj[2*N-2].size()-1});
        } else if (pts[i] + 1 < pts[N - 1]) {
            const int diff = pts[N - 1] - pts[i] - 1;
            adj[2*i].push_back(st{2*i+1, diff, (int)adj[2*i+1].size()});
            adj[2*i+1].push_back(st{2*i, 0, (int)adj[2*i].size()-1});
        }
        adj[2*i+1].push_back(st{2*N-1, 1000000, (int)adj[2*N-1].size()});
        adj[2*N-1].push_back(st{2*i+1, 0, (int)adj[2*i+1].size()-1});
    }
    return get_max_flow(move(adj), 2*N-2, 2*N-1) == need;
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        int N; cin >> N;
        if (N == -1) break;
        cout << (do_testcase(N) ? "YES" : "NO") << '\n';
    }
}
