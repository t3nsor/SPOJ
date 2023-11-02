// 2023-11-01
// There are two basic approaches to this problem. One is to binary search for
// the smallest activation energy required to create all the products; on each
// iteration of the binary search, we do a single BFS treating all the reactants
// as a single node. A different approach is given in jazzyjob-2.cpp.
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
    vector<int> unique_weights = {0, 1000000001};
    for (int i = 0; i < R; i++) {
        int r, p, a; scanf("%d %d %d", &r, &p, &a);
        edges.emplace_back(a, r, p);
        unique_weights.push_back(a);
    }
    if (S == 0) {
        if (D == 0) {
            puts("0 0");
        } else {
            puts("Excessive Energy.");
        }
        return;
    }
    sort(unique_weights.begin(), unique_weights.end());
    unique_weights.erase(unique(unique_weights.begin(), unique_weights.end()),
                         unique_weights.end());
    int l = 0;
    int r = unique_weights.size() - 1;
    while (r > l) {
        int m = (l + r) / 2;
        vector<vector<int>> adj(N);
        for (const auto& edge : edges) {
            if (get<0>(edge) <= unique_weights[m]) {
                adj[get<1>(edge)].push_back(get<2>(edge));
            }
        }
        for (int i = 1; i < S; i++) {
            adj[reactants[0]].push_back(reactants[i]);
        }
        int products_remaining = D;
        queue<int> Q;
        vector<char> vis(N);
        Q.push(reactants[0]);
        bool success = false;
        while (!Q.empty()) {
            const int u = Q.front();
            Q.pop();
            if (vis[u]) continue;
            vis[u] = 1;
            products_remaining -= product_multiplicity[u];
            if (!products_remaining) {
                success = true;
                break;
            }
            for (const auto v : adj[u]) {
                Q.push(v);
            }
        }
        if (success) {
            r = m;
        } else {
            l = m + 1;
        }
    }
    if (l == unique_weights.size() - 1) {
        puts("Excessive Energy.");
        return;
    }
    printf("%d ", unique_weights[l]);
    vector<vector<int>> adj(N);
    for (const auto& edge : edges) {
        if (get<0>(edge) <= unique_weights[l]) {
            adj[get<1>(edge)].push_back(get<2>(edge));
        }
    }
    vector<int> dist(N, -1);
    queue<pair<int, int>> Q;
    for (int i = 0; i < S; i++) {
        Q.emplace(reactants[i], 0);
    }
    int products_remaining = D;
    long long result = 0;
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
