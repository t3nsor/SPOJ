// 2026-06-23
// from the comments: "It can be proven than for every optimal network [sic]
// average connection time is the same."
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
double dfs(int p, int u, double maxd,
           const vector<int>& X, const vector<int>& Y, const vector<int>& pop,
           const vector<vector<int>>& adj) {
    double result = pop[u]*maxd;
    for (const auto v : adj[u]) {
        if (v == p) continue;
        const double d = hypot(X[v] - X[u], Y[v] - Y[u]);
        result += dfs(u, v, max(maxd, d), X, Y, pop, adj);
    }
    return result;
}
double do_testcase(int V) {
    vector<int> X(V), Y(V), pop(V);
    int tpop = 0;
    // I only remember Kruskal's algorithm
    vector<pair<double, pair<int, int>>> edge;
    vector<int> id(V);
    for (int i = 0; i < V; i++) {
        scanf("%d %d %d", &X[i], &Y[i], &pop[i]);
        tpop += pop[i];
        for (int j = 0; j < i; j++) {
            edge.emplace_back(hypot(X[i] - X[j], Y[i] - Y[j]), make_pair(i, j));
        }
        id[i] = i;
    }
    sort(edge.begin(), edge.end());
    vector<vector<int>> adj(V);
    for (const auto& e : edge) {
        int u = e.second.first, v = e.second.second;
        while (u != id[u]) u = id[u] = id[id[u]];
        while (v != id[v]) v = id[v] = id[id[v]];
        if (id[u] != id[v]) {
            id[u] = v;
            adj[e.second.first].push_back(e.second.second);
            adj[e.second.second].push_back(e.second.first);
        }
    }
    return dfs(-1, 0, 0, X, Y, pop, adj) / tpop;
}
int main() {
    for (int cs = 1;; cs++) {
        int V; scanf("%d", &V);
        if (V == 0) break;
        printf("Island Group: %d Average %.2f\n\n", cs, do_testcase(V));
    }
}
