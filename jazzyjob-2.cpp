// 2023-11-01
// There are two basic approaches to this problem. In this one, we build up the
// graph incrementally. Initially, only the reactant nodes are considered
// visited. We insert edges in order of increasing weight, and every time we do,
// if the source vertex of the edge has been visited but the destination has
// not, then we BFS out from the destination vertex as much as we can with the
// edges that exist in the graph so far. Once all the products are visited, we
// have to add in all remaining edges that have the same weight as the last edge
// we added, then repeat the BFS since some distances might have gotten smaller.
// For a different approach, see jazzyjob-1.cpp (but this one is faster).
#include <algorithm>
#include <queue>
#include <stdio.h>
#include <tuple>
#include <vector>
using namespace std;
void do_testcase() {
    int S, D, N; scanf("%d %d %d", &S, &D, &N);
    vector<int> reactants;
    vector<int> products;
    vector<vector<int>> adj(N);
    for (int i = 0; i < S; i++) {
        int x; scanf("%d", &x);
        reactants.push_back(x);
    }
    vector<char> product_multiplicity(N);
    for (int i = 0; i < D; i++) {
        int x; scanf("%d", &x);
        products.push_back(x);
        product_multiplicity[x]++;
    }
    int R; scanf("%d", &R);
    vector<tuple<int, int, int>> edges;
    for (int i = 0; i < R; i++) {
        int r, p, a; scanf("%d %d %d", &r, &p, &a);
        edges.emplace_back(a, r, p);
    }
    sort(edges.begin(), edges.end());
    if (S == 0) {
        if (D == 0) {
            puts("0 0");
        } else {
            puts("Excessive Energy.");
        }
        return;
    }
    int products_remaining = D;
    vector<char> vis(N);
    for (const int r : reactants) {
        products_remaining -= product_multiplicity[r];
        vis[r] = 1;
    }
    if (!products_remaining) {
        puts("0 0");
        return;
    }
    for (int i = 0; i < edges.size(); i++) {
        const int u = get<1>(edges[i]);
        const int v = get<2>(edges[i]);
        adj[u].push_back(v);
        if (!vis[u]) continue;
        queue<int> Q;
        Q.push(v);
        while (!Q.empty()) {
            const int v = Q.front();
            Q.pop();
            if (vis[v]) continue;
            vis[v] = 1;
            products_remaining -= product_multiplicity[v];
            if (!products_remaining) {
                break;
            }
            for (const auto w : adj[v]) {
                Q.push(w);
            }
        }
        if (!products_remaining) {
            printf("%d ", get<0>(edges[i]));
            // add in any other edges of the same weight
            for (int j = i + 1;
                 j < edges.size() && get<0>(edges[j]) == get<0>(edges[i]);
                 j++) {
                adj[get<1>(edges[j])].push_back(get<2>(edges[j]));
            }
            break;
        }
    }
    if (products_remaining > 0) {
        puts("Excessive Energy.");
        return;
    }
    vector<int> dist(N, -1);
    queue<pair<int, int>> Q;
    for (int i = 0; i < S; i++) {
        Q.emplace(reactants[i], 0);
    }
    long long result = 0;
    products_remaining = D;
    while (!Q.empty()) {
        const auto p = Q.front();
        Q.pop();
        if (dist[p.first] != -1) continue;
        dist[p.first] = p.second;
        products_remaining -= product_multiplicity[p.first];
        result += product_multiplicity[p.first] * (long long)p.second;
        if (!products_remaining) {
            printf("%lld\n", result);
            return;
        }
        for (const auto v : adj[p.first]) {
            Q.emplace(v, p.second + 1);
        }
    }
    throw;
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
