// 2024-03-18
// This problem is quite difficult. It's a version of the weighted matching
// problem, but with bounds too high for a mincost flow approach (which will
// have running time ~O(VE)) or the unmodified Hungarian algorithm to pass
// (though there is one comment on the problem suggesting that a mincost flow
// implementation using the ZKW algorithm can get accepted). The trick to
// solving this problem, as explained by some Chinese folks online, is to use a
// modified version of the Hungarian algorithm. Consider the graph as a
// directed graph with 2V vertices where there is an edge from a left vertex i
// to a right vertex j if the edge (i, j) is in the original graph. The K-path
// cover is built up by a series of successive augmentations: initially there
// are no matched pairs, after the first step there will be one matched pair,
// and so on until we have K matched pairs or cannot continue because no
// K-path cover exists. When there are some matched pairs, the residual graph is
// obtained by removing from the graph the edges used in the matching and
// inserting corresponding back edges. To add a new pair, we try searching for
// an augmenting path from some unmatched left vertex to some unmatched right
// vertex in the residual graph. Among all such augmenting paths, we pick the
// one that gives the cheapest resulting matching. The algorithm can be made
// efficient because of the special nature of the edge weights: each edge
// weight is always the sum of its two vertex costs, and augmenting using a
// path that starts at vertex `i` and ends at vertex `j` will always add
// exactly `cost[i] + cost[j]` to the current matching cost. Thus, finding the
// cheapest augumenting path at each step is equivalent to finding the cheapest
// vertex pair that are connected in the residual graph. And the key insight is
// that during each of the K steps of the algorithm, when searching for the
// cheapest augmenting path, if we start from some unmatched vertex `u` on the
// left, pass through some vertex `v` on the right, and end at some unmatched
// vertex `w` on the right (giving an augmenting path that will add
// `cost[u] + cost[w]` to the current cost) then we will never get a cheaper
// path that starts from some vertex `u'` with cost[u'] >= cost[u] and that also
// passes through `v`, because the final vertices that can be reached from `v`
// don't depend on which left unmatched vertex we started from. This implies
// that if we iterate through the unmatched vertices on the left in order of
// their cost, then when searching for an augmenting path starting at such a
// vertex, we shouldn't visit any node on the right that we've already visited
// when starting from a different node earlier in this step. Thus, each of the K
// steps of the algorithm only costs O(E + V) and not O(V(E + V)), giving an
// overall running time of O(K(E + V) + V log V).
#include <algorithm>
#include <queue>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
void do_testcase() {
    int k, V, E; scanf("%d %d %d", &k, &V, &E);
    vector<int> cost(V);
    vector<int> vorder(V);
    for (int i = 0; i < V; i++) {
        scanf("%d", &cost[i]);
        vorder[i] = i;
    }
    sort(vorder.begin(), vorder.end(),
         [&cost] (int x, int y) { return cost[x] < cost[y]; });
    vector<vector<int>> adj(V);
    for (int i = 0; i < E; i++) {
        int u, v; scanf("%d %d", &u, &v); --u; --v;
        adj[u].push_back(v);
    }
    vector<int> fwd(V, -1);  // fwd[i] is the partner of left node i
    vector<int> bck(V, -1);  // bck[i] is the partner of right node i
    vector<int> from(V);
    long long result = 0;
    for (int i = 0; i < k; i++) {
        fill(from.begin(), from.end(), -1);
        int left, right;
        int mincost = 1e9;
        for (const int u : vorder) {
            if (fwd[u] >= 0) continue;
            queue<int> Q;
            Q.push(u);
            while (!Q.empty()) {
                const int v = Q.front();
                Q.pop();
                for (const int w : adj[v]) {
                    if (from[w] >= 0) continue;
                    from[w] = v;
                    if (bck[w] < 0) {
                        if (cost[u] + cost[w] < mincost) {
                            mincost = cost[u] + cost[w];
                            left = u;
                            right = w;
                        }
                    } else {
                        Q.push(bck[w]);
                    }
                }
            }
        }
        if (right < 0) {
            puts("NONE");
            return;
        }
        result += mincost;
        do {
            bck[right] = from[right];
            swap(right, fwd[bck[right]]);
        } while (right >= 0);
    }
    printf("%lld\n", result);
}
int main() {
    int c; scanf("%d", &c);
    while (c--) do_testcase();
}
