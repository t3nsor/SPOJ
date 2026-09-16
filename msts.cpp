// 2026-09-16
// There are two algorithms for this problem: the obvious one and the clever
// one.  This file implements the obvious solution; I'll explain the clever one
// at the end of this comment block.
//
// For a given graph G and MST T of G, T can be produced by Kruskal's algorithm
// if the edges of G are sorted such that an edge belonging to T sorts before
// any equally-weighted edge that doesn't belong to T.  Thus, counting MSTs is
// equivalent to counting the number of possible outcomes of Kruskal's.  Let `w`
// be some weight value.  Let w_min be the smallest edge weight in G.  When
// Kruskal's algorithm finishes processing all edges of weight w_min, it has
// produced some forest with one spanning tree for each connected component in
// the graph G(w_min) which consists of only the edges in G with weight w_min.
// When the algorithm proceeds to heavier edges, it no longer matters which
// edges of weight w_min were selected since the only thing that matters at each
// step is the transitive closure of the already-selected edges, which is the
// same as the transitive closure of *all* already-processed edges.  Thus, the
// number of possible outcomes is the product of N(w_i) for all distinct weights
// w_i, where N(w_i) is defined as the number of possible spanning forests of
// the graph G(w_i) which is defined as follows: each node of G(w_i) is a set of
// nodes in G that are connected to each other by edges of weight less than w_i,
// and there is an edge between two nodes U, V of G(w_i) if there is an edge of
// weight w_i between two nodes u, v in G where u ∈ U, v ∈ V.  To count spanning
// forests in each case, we use the matrix-tree theorem.  Finally note that
// 31011 = 3*10337 so we have to compute each determinant twice, once mod 3 and
// once mod 10337, so we can reconstruct what it should be mod 31011.
//
// The clever solution (which this file doesn't implement) involves computing
// only a single matrix determinant, but needs a trick that is hard to find.
// Instead of the usual Laplacian matrix which is obtained from the degree
// (i.e. number of incident edges) for each node, we construct a matrix that
// takes into account the weights of edges.  Let w(i, j) be the weight of the
// edge between i and j; then define
//
//   M_{i,j} = -x^{w(i, j)},       if i != j and there is an edge (i, j)
//             0,                  if i != j and there is no edge (i, j)
//             \sum_k x^{w(i, k)}, if i = j, where k ranges over all nodes such
//                                 that there is an edge (i, k)
//
// where i, j range from 0 to V - 1.  (Note that if all weights are 0 then M is
// just the usual Laplacian with its last row and column deleted.)  The
// determinant of M is the generating function for the number of spanning trees
// trees with a given total weight.  In general this polynomial will have an
// exponential size and computing it explicitly is too inefficient, but we care
// only about the coefficient of the term x^m where m is the weight of any MST.
// Ideally we could divide out factors of x from rows and columns of M until
// we're left with det M = x^m det M' at which point we could drop all terms
// with positive degree from M'.  This is the clever part: because the rows and
// columns of M may not initially allow us to divide out m factors of x, we
// first perform a sequence of elementary column operations until it's possible
// to factor out a total of m factors of x from the columns of M.  This is done
// using "Algorithm A" from "Counting Minimum Weight Spanning Trees" by Broder
// and Mayr (1995):
// 1. Assume the nodes are labelled 1, ..., V.
// 2. Let T be an arbitrary spanning tree of the graph, rooted at node V.
// 3. While T contains more than one node:
//    1. Choose a leaf u whose label is not V.
//    2. Let p be the parent of u in T.
//    3. If p is not V, add column u to column p.
//    4. Delete u from T.
// When this is done, we will magically be able to divide out m factors of x
// from the resulting M.  (The paper also explains how to execute Algorithm A in
// time O(V^2 + E log V), as a result of which the total running time is always
// asymptotically dominated by the complexity of matrix multiplication.
// However, if I were implementing this algorithm, I would use the naive O(V^3)
// Gaussian elimination anyway, so there would be no point in optimizing the
// running time of Algorithm A to be less than O(VE).)

#include <iostream>
#include <map>
#include <utility>
#include <vector>
using namespace std;
const int MOD = 31011;
const int MOD1 = 3;
const int MOD2 = 10337;
int inv1[MOD1];
int inv2[MOD2];
int pmod(int x, int e, int mod) {
    if (e == 0) return 1;
    int r = pmod(x, e / 2, mod);
    r = (r * r) % mod;
    if (e & 1) r = (r * x) % mod;
    return r;
}
int calc_inv(int x, int mod) {
    return pmod(x, mod - 2, mod);
}
void dfs(const vector<vector<int>>& adj,
         int u,
         vector<int>& id_to_id2,
         vector<int>& local_id,
         int id2,
         int& size2) {
    if (id_to_id2[u] >= 0) return;
    id_to_id2[u] = id2;
    local_id[u] = size2++;
    for (const auto& v : adj[u]) dfs(adj, v, id_to_id2, local_id, id2, size2);
}
int det(vector<vector<int>> A, int mod, int* inv) {
    const int N = A.size();
    int result = 1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = (A[i][j] % mod + mod) % mod;
        }
    }
    for (int i = 0; i < N; i++) {
        int j = i;
        while (j < N && A[j][i] == 0) ++j;
        if (j == N) return 0;
        if (j != i) { swap(A[i], A[j]); result = (mod - result) % mod; }
        result = (result * A[i][i]) % mod;
        const int f = inv[A[i][i]];
        for (int j = i; j < N; j++) A[i][j] = (A[i][j] * f) % mod;
        for (int j = 0; j < N; j++) {
            if (j == i) continue;
            const int f2 = (mod - A[j][i]) % mod;
            for (int k = i; k < N; k++) {
                A[j][k] = (A[j][k] + f2*A[i][k]) % mod;
            }
        }
    }
    return result;
}
int matrix_tree(const vector<vector<int>>& m) {
    int N = m.size();
    vector<vector<int>> M(N - 1, vector<int>(N - 1));
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N; j++) {
            M[i][i] += m[i][j];
        }
        for (int j = 0; j < N - 1; j++) {
            if (i != j) M[i][j] = -m[i][j];
        }
    }
    const int rem1 = det(M, MOD1, inv1);
    const int rem2 = det(M, MOD2, inv2);
    int result = rem2;
    while (result % MOD1 != rem1) result += MOD2;
    return result;
}
int main() {
    ios::sync_with_stdio(false);
    for (int i = 1; i < MOD1; i++) inv1[i] = calc_inv(i, MOD1);
    for (int i = 1; i < MOD2; i++) inv2[i] = calc_inv(i, MOD2);
    map<int, vector<pair<int, int>>> EM;
    int V, E; cin >> V >> E;
    while (E--) {
        int u, v, w; cin >> u >> v >> w;
        EM[w].emplace_back(u - 1, v - 1);
    }
    vector<int> id(V);
    for (int i = 0; i < V; i++) id[i] = i;
    int nc = V;
    int result = 1;
    for (const auto& kv : EM) {
        const auto& edges = kv.second;
        bool can_use = false;
        // optimization: skip expensive computations if no edge with this weight
        // can be used (so we only do them O(V) times)
        for (const auto& e : edges) {
            if (id[e.first] != id[e.second]) { can_use = true; break; }
        }
        if (!can_use) continue;
        // generate adjacency list
        vector<vector<int>> adj(nc);
        for (const auto& e : edges) {
            if (id[e.first] != id[e.second]) {
                adj[id[e.first]].push_back(id[e.second]);
                adj[id[e.second]].push_back(id[e.first]);
            }
        }
        // compute new components
        vector<int> id_to_id2(nc, -1);
        vector<int> size2;
        vector<int> local_id(nc);
        int nc2 = 0;
        for (int i = 0; i < V; i++) {
            if (id_to_id2[id[i]] >= 0) continue;
            size2.push_back(0);
            dfs(adj, id[i], id_to_id2, local_id, nc2++, size2.back());
        }
        // generate adjacency matrices
        vector<vector<vector<int>>> ms;
        for (int i = 0; i < nc2; i++) {
            ms.push_back(vector<vector<int>>(size2[i], vector<int>(size2[i])));
        }
        for (const auto& e : edges) {
            if (id[e.first] == id[e.second]) continue;
            auto& m = ms[id_to_id2[id[e.first]]];
            m[local_id[id[e.first]]][local_id[id[e.second]]]++;
            m[local_id[id[e.second]]][local_id[id[e.first]]]++;
        }
        for (int i = 0; i < nc2; i++) {
            result = (result * matrix_tree(ms[i])) % MOD;
        }
        for (int i = 0; i < V; i++) id[i] = id_to_id2[id[i]];
        nc = nc2;
    }
    cout << result << '\n';
}
