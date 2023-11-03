// 2023-11-02
// This is an instance of the closure problem:
// https://en.wikipedia.org/wiki/Closure_problem
// It's referred to as "Transportation network design" in the above article.
// If we construct a graph where each edge A_i <-> B_i is represented by a
// node with positive weight C_i, with an outward edge to each of the two nodes
// A_i, B_i (which have negative weight, -P_{A_i} and -P_{B_i}), then the max
// profit equals the maximum weight closure in this graph, which can be reduced
// to finding a minimum cut in a modified graph.
//
// We use Dinic's algorithm, implementation taken from the Stanford team
// notebook, because Brian's attempt at implementing the relabel-to-front
// algorithm got TLE. There is no known license for this code, so tread with
// caution if you decide to reuse it.
#include <iostream>
#include <vector>
using namespace std;
const int INF = 2000000000;
struct Edge {
    int from, to, cap, flow, index;
    Edge(int from, int to, int cap, int flow, int index) :
        from(from), to(to), cap(cap), flow(flow), index(index) {}
};

struct Dinic {
    int N;
    vector<vector<Edge> > G;
    vector<Edge *> dad;
    vector<int> Q;

    // N = number of vertices
    Dinic(int N) : N(N), G(N), dad(N), Q(N) {}

    // Add an edge to initially empty network. from, to are 0-based
    void AddEdge(int from, int to, int cap) {
        G[from].push_back(Edge(from, to, cap, 0, G[to].size()));
        if (from == to) G[from].back().index++;
        G[to].push_back(Edge(to, from, 0, 0, G[from].size() - 1));
    }

    long long BlockingFlow(int s, int t) {
        fill(dad.begin(), dad.end(), nullptr);
        dad[s] = &G[0][0] - 1;

        int head = 0, tail = 0;
        Q[tail++] = s;
        while (head < tail) {
            int x = Q[head++];
            for (int i = 0; i < G[x].size(); i++) {
                Edge &e = G[x][i];
                if (!dad[e.to] && e.cap - e.flow > 0) {
                    dad[e.to] = &G[x][i];
                    Q[tail++] = e.to;
                }
            }
        }
        if (!dad[t]) return 0;

        long long totflow = 0;
        for (int i = 0; i < G[t].size(); i++) {
            Edge *start = &G[G[t][i].to][G[t][i].index];
            int amt = INF;
            for (Edge *e = start; amt && e != dad[s]; e = dad[e->from]) {
                if (!e) { amt = 0; break; }
                amt = min(amt, e->cap - e->flow);
            }
            if (amt == 0) continue;
            for (Edge *e = start; amt && e != dad[s]; e = dad[e->from]) {
                e->flow += amt;
                G[e->to][e->index].flow -= amt;
            }
            totflow += amt;
        }
        return totflow;
    }

    // Call this to get the max flow. s, t are 0-based.
    // Note, you can only call this once.
    // To obtain the actual flow values, look at all edges with
    // capacity > 0 (zero capacity edges are residual edges).
    long long GetMaxFlow(int s, int t) {
        long long totflow = 0;
        while (long long flow = BlockingFlow(s, t))
            totflow += flow;
        return totflow;
    }
};
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        int n, m; scanf("%d %d", &n, &m);
        Dinic d(n + m + 2);
        for (int i = 1; i <= n; i++) {
            int cost; scanf("%d", &cost);
            d.AddEdge(i, n + m + 1, cost);
        }
        int total = 0;
        for (int i = 1; i <= m; i++) {
            int a, b, profit; scanf("%d %d %d", &a, &b, &profit);
            total += profit;
            d.AddEdge(0, n + i, profit);
            d.AddEdge(n + i, a, INF);
            d.AddEdge(n + i, b, INF);
        }
        printf("%lld\n", total - d.GetMaxFlow(0, n + m + 1));
    }
}
