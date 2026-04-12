// 2026-04-12
// Brian's implementation of the "push-relabel with cost scaling" algorithm for
// mincost maxflow.
#include <algorithm>
#include <limits.h>
#include <queue>
#include <utility>
#include <vector>
class MCMF {
    using LL = long long;
    int V;
    std::vector<LL> pot;
    std::vector<LL> excess;
    LL eps;

  public:
    struct Edge {
        int to;
        int rev;
        LL res;
        LL cost;
        LL flow;
    };
    std::vector<std::vector<Edge>> graph;

    // call this before starting a new graph (including the first time)
    void init(int V) {
        this->V = V;
        graph.clear();
        graph.resize(V);
        eps = 1;
    }

    void add_edge(int u, int v, LL cap, LL cost) {
        graph[u].push_back(Edge{v, (int)graph[v].size(), cap, cost});
        graph[v].push_back(Edge{u, (int)graph[u].size() - 1, 0, -cost});
        while (eps < abs(cost)) eps <<= 1;
    }

    // call this exactly once for a given graph; the output consists of the
    // `flow` fields in `graph`
    void calc(int s, int t) {
        // calculate the max flow
        auto graph_copy = graph;
        const LL F = calc_maxflow(s, t);
        graph = std::move(graph_copy);
        // set demand values
        excess.assign(V, 0);
        excess[s] = F;
        excess[t] = -F;
        pot.assign(V, 0);
        // scale the costs (the scale factor must be strictly greater than `V`
        // in order for this to be correct)
        LL scale = 1; while (scale <= V) scale <<= 1;
        for (int i = 0; i < V; i++) for (auto& e : graph[i]) e.cost *= scale;
        eps *= scale;
        while (eps >>= 1) { refine(); }
        // restore original costs
        for (int i = 0; i < V; i++) for (auto& e : graph[i]) e.cost /= scale;
    }

  private:
    void augment(int from, Edge& e, LL delta) {
        auto& r = graph[e.to][e.rev];
        e.flow += delta;
        r.flow -= delta;
        e.res -= delta;
        r.res += delta;
        excess[from] -= delta;
        excess[e.to] += delta;
    }

    LL calc_maxflow(int s, int t) {
        // optimized push-relabel (adapted from fastflow.cpp; see there for an
        // explanation)
        pot.assign(V, 0);
        excess.assign(V, 0);
        std::vector<int> active(V, 0);
        std::vector<int> hcnt(2*V + 1, 0);
        std::queue<int> Q;
        pot[s] = V;
        hcnt[V] = 1;
        hcnt[0] = V - 1;
        for (int i = 0; i < V; i++) {
            if (i == s || i == t) continue;
            active[i] = 1;
            Q.push(i);
        }
        for (auto& e : graph[s]) {
            excess[e.to] += e.res;
            graph[e.to][e.rev].res = std::exchange(e.res, 0);
        }
        while (!Q.empty()) {
            const int u = Q.front();
            Q.pop();
            active[u] = 0;
            int best = 1e9;
            for (auto& e : graph[u]) {
                if (!e.res) continue;
                if (pot[u] == pot[e.to] + 1) {
                    const int x = std::min<long long>(excess[u], e.res);
                    excess[u] -= x;
                    excess[e.to] += x;
                    e.res -= x;
                    graph[e.to][e.rev].res += x;
                    if (e.to != s && e.to != t && !active[e.to]) {
                        active[e.to] = 1;
                        Q.push(e.to);
                    }
                    if (excess[u] == 0) break;
                } else {
                    best = std::min(best, (int)pot[e.to]);
                }
            }
            if (excess[u] > 0) {
                const int oldh = pot[u];
                pot[u] = best + 1;
                hcnt[best + 1]++;
                if (0 == --hcnt[oldh] && oldh < V) {
                    for (int i = 0; i < V; i++) {
                        if (i != s && i != t &&
                            pot[i] > oldh && pot[i] <= V) {
                            hcnt[pot[i]]--;
                            pot[i] = V + 1;
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

    void refine() {
        for (int i = 0; i < V; i++) {
            for (auto& e : graph[i]) {
                if (e.res > 0 && e.cost + pot[i] - pot[e.to] < 0) {
                    augment(i, e, e.res);
                }
            }
        }
        std::queue<int> Q;
        for (int i = 0; i < V; i++) { if (excess[i] > 0) Q.push(i); }
        while (!Q.empty()) {
            const int u = Q.front(); Q.pop();
            while (excess[u] > 0) {
                bool adv = false;
                for (auto& e : graph[u]) {
                    if (e.res > 0 && e.cost + pot[u] - pot[e.to] < 0) {
                        const LL delta = std::min(excess[u], e.res);
                        const LL old_excess = excess[e.to];
                        augment(u, e, delta);
                        if (old_excess <= 0 && excess[e.to] > 0) Q.push(e.to);
                        adv = true;
                        if (!excess[u]) break;
                    }
                }
                if (excess[u] > 0 && !adv) {
                    LL m = LLONG_MAX;
                    for (const auto& e : graph[u]) {
                        if (e.res > 0) {
                            m = std::min(m, e.cost + pot[u] - pot[e.to]);
                        }
                    }
                    pot[u] -= m + eps;
                }
            }
        }
    }
};

// driver code starts here
#include <iostream>
using namespace std;

void do_testcase() {
    int N; cin >> N;
    MCMF mcmf;
    mcmf.init(N + 2);
    for (int i = 0; i < N; i++) {
        mcmf.add_edge(i + 1, N + 1, 1, 0);
        int x; cin >> x;
        mcmf.add_edge(0, x, 1, 0);
    }
    int E; cin >> E;
    while (E--) {
        int x, y; cin >> x >> y;
        mcmf.add_edge(x, y, N, 1);
        mcmf.add_edge(y, x, N, 1);
    }
    mcmf.calc(0, N + 1);
    long long cost = 0;
    for (int i = 0; i < N + 2; i++) {
        for (const auto& e : mcmf.graph[i]) {
            cost += e.flow * e.cost;
        }
    }
    cout << cost / 2 << '\n';
}

int main() {
    int T; cin >> T; while (T--) do_testcase();
}
