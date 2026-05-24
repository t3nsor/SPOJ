// 2026-05-24
// The proof that the following algorithm is correct is left as an exercise for
// the reader: sort the given vertices in descending order of degree, breaking
// ties arbitrarily, and for each prefix of the resulting list, attempt to form
// a league with those vertices, checking that it satisfies the conditions that
// there is an edge between every pair of nodes in the subset and no edges
// between any pair of nodes in its complement.  We can, of course, give up once
// the subset grows large enough that the first condition is no longer
// satisfied, which will occur within O(sqrt(E)) steps.
#include <algorithm>
#include <stdio.h>
#include <unordered_set>
#include <vector>
using namespace std;
bool do_testcase(int V, int E) {
    vector<unordered_set<int>> adj(V);
    // call an edge "bad" if both endpoints are not in the current clique
    int bad = E;
    while (E--) {
        int u, v; scanf("%d %d", &u, &v); --u; --v;
        adj[u].insert(v); adj[v].insert(u);
    }
    vector<int> order(V);
    for (int i = 0; i < V; i++) order[i] = i;
    sort(order.begin(), order.end(),
         [&](int i, int j) { return adj[i].size() > adj[j].size(); });
    for (int i = 0; i < V; i++) {
        const int u = order[i];
        // ensure it's connected to all previous nodes in the list
        for (int j = 0; j < i; j++) {
            const int v = order[j];
            if (!adj[u].count(v)) return false;
        }
        // reclassify all other incident edges as non-bad
        bad -= adj[u].size() - i;
        if (bad == 0) return true;
    }
    return false;
}
int main() {
    for (;;) {
        int V, E; scanf("%d %d", &V, &E); if (V == 0) break;
        puts(do_testcase(V, E) ? "Y" : "N");
    }
}
