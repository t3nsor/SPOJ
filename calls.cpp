// 2024-01-07
// First we need to get the obvious cases out of the way: if K is greater than
// the number of pairs of cities, the flyer is a forgery. If the advertised cost
// between a pair of cities (i, j) is greater than the cost (i, k) plus the cost
// (k, j), then the flyer is a forgery.
//
// After that it becomes a question of whether we can reduce the given graph to
// K edges such that the shortest path matrix of the reduced graph is equal to
// that of the original graph. Because all costs are strictly positive, there is
// a simple greedy approach: always try to remove the highest weight edge first
// (it can be removed if there is an indirect path that reproduces the same
// cost) because if we can remove it, then it can't be optimal to leave it in
// (it can't form part of the shortest path between any other pair of vertices).
// Then go on to the next highest weight edge, and so on. (In contrast, the
// unweighted transitive reduction problem (finding the graph with the fewest
// edges that has the same transitive closure as the original graph) is more
// complicated: there's no obvious order in which to remove edges.)
#include <algorithm>
#include <stdio.h>
#include <tuple>
#include <vector>
using namespace std;
void do_testcase() {
    int N, K; scanf("%d %d", &N, &K);
    vector<vector<int>> a(N, vector<int>(N, 0));
    vector<tuple<int, int, int>> v;
    int edges_used = N * (N - 1) / 2;
    if (K > edges_used) {
        // e.g. 3 cities with 4 phone lines would be impossible
        puts("NO");
        return;
    }
    for (int i = 0; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            scanf("%d", &a[i][j]);
            a[j][i] = a[i][j];
            v.emplace_back(a[i][j], i, j);
        }
    }
    // check for consistency
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            for (int k = 0; k < N; k++) {
                if (a[i][j] > a[i][k] + a[k][j]) {
                    puts("NO");
                    return;
                }
            }
        }
    }
    if (K == edges_used) {
        puts("YES");
        return;
    }
    sort(v.begin(), v.end(), greater<>());
    for (const auto& t : v) {
        const int u = get<1>(t);
        const int v = get<2>(t);
        bool remove = false;
        for (int w = 0; w < N; w++) {
            if (a[u][v] == a[u][w] + a[w][v] && w != u && w != v) {
                remove = true;
                break;
            }
        }
        if (remove) {
            --edges_used;
            if (edges_used == K) {
                puts("YES");
                return;
            }
            a[u][v] = a[v][u] = 1e9;
        }
    }
    puts("NO");
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
