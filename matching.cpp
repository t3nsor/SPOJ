// 2023-11-01
// Use Dinic's algorithm, taken from Stanford's team notebook. There is no known
// license for this code, so tread with caution if reusing it.
#include <stdio.h>
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
    int N, M, P; scanf("%d %d %d", &N, &M, &P);
    Dinic d(2 + N + M);
    for (int i = 1; i <= N; i++) {
        d.AddEdge(0, i, 1);
    }
    for (int i = 1; i <= M; i++) {
        d.AddEdge(N + i, N + M + 1, 1);
    }
    for (int i = 0; i < P; i++) {
        int A, B; scanf("%d %d", &A, &B);
        d.AddEdge(A, N + B, 1);
    }
    printf("%d\n", (int)d.GetMaxFlow(0, N + M + 1));
    return 0;
}