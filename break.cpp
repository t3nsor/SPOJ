// 2026-09-21
// The official solution on IPSC suggests to run one DFS or BFS from each node.
// This is probably too slow for SPOJ (V <= 9000, E <= 52000).  By finding the
// strongly connected components and producing the kernel DAG, we can use
// dynamic programming, which doesn't actually reduce the complexity, but allows
// us to use bitmasks so that when each edge is processed (exactly once, as long
// as you remember to remove duplicates) we can perform V/W operations instead
// of W (where W is the word size).  To convert the resulting bitmasks to counts
// takes O(V^2) time but another bitmask trick (see below) can be used to cut
// the number of required operations by a factor of 8.
//
// Some in the comments claim that a linear time solution exists but I'm not
// sure if I believe it.
#include <algorithm>
#include <array>
#include <stdio.h>
#include <queue>
#include <vector>
using namespace std;
void dfs1(const vector<vector<int>>& adj, int u, int& cur, vector<int>& pn) {
    if (pn[u] >= 0) return;
    pn[u] = 1e9;
    for (const auto& v : adj[u]) dfs1(adj, v, cur, pn);
    pn[u] = cur++;
}
void dfs2(const vector<vector<int>>& adj, int u, int& cur, vector<int>& sid) {
    if (sid[u] >= 0) return;
    sid[u] = cur;
    for (const auto& v : adj[u]) dfs2(adj, v, cur, sid);
}
void do_testcase() {
    int V, E; scanf("%d %d", &V, &E);
    vector<vector<int>> adj(V);
    vector<vector<int>> radj(V);
    while (E--) {
        int u, v; scanf("%d %d", &u, &v); --u; --v;
        adj[u].push_back(v);
        radj[v].push_back(u);
    }
    vector<int> postnum(V, -1);
    int cur = 0;
    for (int i = 0; i < V; i++) dfs1(radj, i, cur, postnum);
    vector<int> postorder(V);
    for (int i = 0; i < V; i++) postorder[postnum[i]] = i;
    vector<int> sid(V, -1);
    int nscc = 0;
    for (int i = V - 1; i >= 0; i--) {
        const int u = postorder[i];
        if (sid[u] >= 0) continue;
        dfs2(adj, u, nscc, sid);
        nscc++;
    }
    vector<int> sccsize(nscc);
    for (int i = 0; i < V; i++) sccsize[sid[i]]++;
    vector<vector<int>> kadj(nscc);
    for (int u = 0; u < V; u++) {
        for (const auto v : adj[u]) {
            if (sid[u] != sid[v]) kadj[sid[u]].push_back(sid[v]);
        }
    }
    vector<int> indeg(nscc);
    for (int i = 0; i < nscc; i++) {
        sort(kadj[i].begin(), kadj[i].end());
        kadj[i].erase(unique(kadj[i].begin(), kadj[i].end()), kadj[i].end());
        for (const auto j : kadj[i]) ++indeg[j];
    }
    const int nwords = (nscc + 63) / 64;
    using ULL = unsigned long long;
    vector<vector<ULL>> mask(nscc);
    queue<int> Q;
    for (int i = 0; i < nscc; i++) {
        if (indeg[i] == 0) {
            Q.push(i);
            mask[i].assign(nwords, 0);
        }
    }
    // optimization: build lookup tables for bitmasks of length 8 of SCCs so we
    // don't have to iterate through every single bit
    vector<array<int, 256>> masksum((nscc + 7) / 8);
    for (int i = 0; i < (nscc + 7) / 8; i++) {
        const int cnt = min(8, nscc - 8*i);
        for (int j = 1; j < (1 << cnt); j++) {
            const int pos = __builtin_ctz(j);
            masksum[i][j] = masksum[i][j - (1 << pos)] + sccsize[8*i + pos];
        }
    }
    vector<int> score(nscc, 0);
    int maxscore = 0;
    while (!Q.empty()) {
        const int x = Q.front(); Q.pop();
        mask[x][x / 64] |= (1ULL << (x % 64));
        for (int i = 0; i < nwords; i++) {
            const ULL curmask = mask[x][i];
            for (int j = 0; j < 8; j++) {
                if (8*i + j < masksum.size()) {
                    score[x] += masksum[8*i + j][(curmask >> (8*j)) & 255];
                }
            }
        }
        maxscore = max(maxscore, score[x]);
        for (const auto y : kadj[x]) {
            if (mask[y].empty()) mask[y].assign(nwords, 0);
            for (int i = 0; i < nwords; i++) mask[y][i] |= mask[x][i];
            if (0 == --indeg[y]) Q.push(y);
        }
    }
    bool first = true;
    for (int i = 0; i < V; i++) {
        if (score[sid[i]] == maxscore) {
            if (!first) putchar(' ');
            first = false;
            printf("%d", i + 1);
        }
    }
    putchar('\n');
}
int main() {
    int T; scanf("%d", &T); while (T--) do_testcase();
}
