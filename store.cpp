// 2023-10-11
// The overall approach is BFS, where pushing the block has weight 1, and
// moving the storekeeper without pushing the block has weight 0. 0-1 BFS is a
// classic problem whose solution uses a deque instead of the usual queue. The
// main issue is how to determine whether the storekeeper can move from one
// node to another without pushing the block.
//
// Answering a query of the form "can I reach v from u without going through p"
// can be done using biconnectivity. Assume that u, v, and p belong to a common
// connected component (but not necessary the same biconnected component). If p
// is not a cut point, then the answer is always yes. If p is a cut point, then
// we have to consider the nodes corresponding to u, v, p in the block-cut
// tree; call them U, V, P, respectively. If the path from U to V goes through
// P, then there is no path if P is removed, so the answer is no. Otherwise,
// the answer is yes. Basically, the block-cut tree allows us to reduce the
// original query to one on a tree. This query on a tree can then be reduced to
// a series of LCA queries (see below).
//
// It is also possible to solve this problem more simply without LCA. Whenever
// we need to check whether the storekeeper can move from one square that is
// adjacent to the package to a different square that is also adjacent to the
// package, we already know that there's a path whose sole intermediate node is
// the package, so a second path that doesn't go through the package exists if
// and only if the two adjacent squares are in the same biconnected component
// (by definition). This can be checked easily since each square can only be
// part of at most 4 biconnected components (the max degree of the original
// graph). (You still need an initial DFS to get from the initial position of
// the storekeeper to each of the squares adjacent to the package, though.)
#include <algorithm>
#include <deque>
#include <iostream>
#include <limits.h>
#include <math.h>
#include <memory>
#include <stack>
#include <string>
#include <string.h>
#include <vector>
using namespace std;
struct Segtree {
    vector<int> data;
    int size;
    Segtree(const vector<int>& initial) {
        size = initial.size();
        // round up n to the nearest power of 2
        if (size > 1) {
            size = 1 << (32 - __builtin_clz(size - 1));
        }
        data.assign(2*size, INT_MAX);
        for (int i = 0; i < initial.size(); i++) {
            data[size + i] = initial[i];
        }
        for (int i = size - 1; i >= 1; i--) {
            data[i] = min(data[2*i], data[2*i + 1]);
        }
    }
    int query_helper(int ql, int qr, int node, int tl, int tr) const {
        if (ql <= tl && qr >= tr) return data[node];
        const int tm = (tl + tr) / 2;
        int result = INT_MAX;
        if (ql < tm && qr > tl) {
            result = min(result, query_helper(ql, qr, 2*node, tl, tm));
        }
        if (ql < tr && qr > tm) {
            result = min(result, query_helper(ql, qr, 2*node + 1, tm, tr));
        }
        return result;
    }
    int query(int l, int r) const {
        return query_helper(l, r, 1, 0, size);
    }
};
struct LCA {
    unique_ptr<const Segtree> segtree;
    vector<int> position;
    vector<int> pre;
    vector<int> invpre;
    void traverse(const vector<vector<int>>& adjlist,
                  int parent,
                  int node,
                  vector<int>& traversal,
                  vector<int>& pre,
                  int& cnt) {
        if (pre[node] == -1) {
            pre[node] = cnt++;
        }
        traversal.push_back(pre[node]);
        for (const int neighbor : adjlist[node]) {
            if (neighbor == parent) continue;
            traverse(adjlist, node, neighbor, traversal, pre, cnt);
            traversal.push_back(pre[node]);
        }
    }
    LCA(const vector<vector<int>>& adjlist) {
        vector<int> traversal;
        pre.assign(adjlist.size(), -1);
        int cnt = 0;
        traverse(adjlist, -1, 0, traversal, pre, cnt);
        invpre.resize(adjlist.size());
        for (int i = 0; i < pre.size(); i++) {
            invpre[pre[i]] = i;
        }
        position.assign(adjlist.size(), -1);
        for (int i = 0; i < traversal.size(); i++) {
            if (position[traversal[i]] < 0) {
                position[traversal[i]] = i;
            }
        }
        segtree = make_unique<Segtree>(traversal);
    }
    int query(int u, int v) {
        u = position[pre[u]];
        v = position[pre[v]];
        if (u > v) swap(u, v);
        return invpre[segtree->query(u, v + 1)];
    }
};
// Find biconnected components reachable from `node`. This code is copied from
// my solution for CHASE1. Note that there is a special case when `node` has no
// outgoing edges: this is not handled by `dfs`, and must be accounted for by
// the caller.
int dfs(int parent, int node,
        const vector<vector<int>>& adjlist,
        vector<vector<int>>& bicomp) {
    thread_local int current_prenum;
    thread_local vector<int> preorder;
    thread_local stack<int> stk;
    if (parent == -1) {
        current_prenum = 0;
        preorder.assign(adjlist.size(), -1);
    }

    preorder[node] = current_prenum++;
    int ret = preorder[node];
    for (const auto neighbor : adjlist[node]) {
        if (neighbor == parent) continue;
        if (preorder[neighbor] >= 0) {
            if (preorder[neighbor] < preorder[node]) {
                ret = min(ret, preorder[neighbor]);
            }
        } else {
            stk.push(neighbor);
            const int child_ret = dfs(node, neighbor, adjlist, bicomp);
            ret = min(ret, child_ret);
            if (parent < 0 ||
                (parent >= 0 && child_ret >= preorder[node])) {
                int v;
                vector<int> current_bicomp;
                do {
                    v = stk.top();
                    current_bicomp.push_back(v);
                    stk.pop();
                } while (v != neighbor);
                current_bicomp.push_back(node);
                bicomp.push_back(move(current_bicomp));
            }
        }
    }
    return ret;
}

const int dr[4] = {0, 1, 0, -1};
const int dc[4] = {1, 0, -1, 0};

void do_testcase() {
    int R, C; cin >> R >> C;
    vector<string> grid(R);
    int mr, mc, sr, sc, fr, fc;
    for (int i = 0; i < R; i++) {
        cin >> grid[i];
        for (int j = 0; j < C; j++) {
            if (grid[i][j] == 'M') {
                mr = i;
                mc = j;
            } else if (grid[i][j] == 'P') {
                sr = i;
                sc = j;
            } else if (grid[i][j] == 'K') {
                fr = i;
                fc = j;
            }
        }
    }
    // compute adjacency list for all non-walls
    vector<vector<int>> adj1(R*C);
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if (grid[i][j] == 'S') continue;
            for (int k = 0; k < 4; k++) {
                const int i2 = i + dr[k];
                const int j2 = j + dc[k];
                if (i2 >= 0 && j2 >= 0 && i2 < R && j2 < C &&
                    grid[i2][j2] != 'S') {
                    adj1[C*i + j].push_back(C*i2 + j2);
                }
            }
        }
    }
    // special case for isolated node
    if (adj1[C*sr+sc].empty()) {
        cout << "NO\n";
        return;
    }
    vector<vector<int>> bicomp;
    dfs(-1, C*sr + sc, adj1, bicomp);
    // determine articulation points
    vector<int> cnt(R*C, 0);
    for (const auto& c : bicomp) {
        for (const int node : c) {
            ++cnt[node];
        }
    }
    // note: cnt[v] >= 2 means v is an articulation point
    vector<int> block_idx(R*C, -1);
    int ap_cnt = 0;
    for (int i = 0; i < R*C; i++) {
        if (cnt[i] >= 2) {
            block_idx[i] = ap_cnt++;
        }
    }
    // build block-cut tree
    int edges = 0;
    vector<vector<int>> adj2(ap_cnt + bicomp.size());
    for (int i = 0; i < bicomp.size(); i++) {
        for (const int v : bicomp[i]) {
            if (cnt[v] >= 2) {
                adj2[block_idx[v]].push_back(ap_cnt + i);
                adj2[ap_cnt + i].push_back(block_idx[v]);
                ++edges;
            } else {
                block_idx[v] = ap_cnt + i;
            }
        }
    }
    // build LCA data structure for block-cut tree
    LCA lca(adj2);
    auto can_reach = [&](int n1, int n2, int p) {
        // Determine whether it's possible to travel from n1 to n2 without
        // going through p. p must be distinct from n1, n2
        if (cnt[n1] < 1 || cnt[n2] < 1) {
            // no connectivity
            return false;
        }
        if (cnt[p] < 2) {
            // p is not an articulation point
            return true;
        }
        const int block1 = block_idx[n1];
        const int block2 = block_idx[n2];
        const int pblock = block_idx[p];
        // Check whether the path from block1 to block2 goes through pblock.
        // If pblock is the LCA, then the path goes through pblock. Otherwise,
        // the path goes through pblock only if pblock is an ancestor of one of
        // the two (without loss of generality, let it be block1), and pblock
        // is on the path from block1 to the LCA, i.e., the LCA of pblock and
        // block2 is the same as the LCA of block1 and block2.
        const int lca12 = lca.query(block1, block2);
        const int lca1p = lca.query(block1, pblock);
        const int lca2p = lca.query(block2, pblock);
        return !(lca12 == pblock ||
                 (lca1p == lca12 && lca2p == pblock) ||
                 (lca1p == pblock && lca2p == lca12));
    };
    int dist[100][100][4];
    memset(dist, -1, sizeof(dist));
    deque<tuple<int, int, int, int>> Q;
    for (int k = 0; k < 4; k++) {
        const int i = sr + dr[k];
        const int j = sc + dc[k];
        if (i >= 0 && j >= 0 && i < R && j < C && grid[i][j] != 'S' &&
            can_reach(C * mr + mc, C * i + j, C * sr + sc)) {
            Q.emplace_front(sr, sc, k, 0);
        }
    }
    while (!Q.empty()) {
        auto n = Q.front();
        Q.pop_front();
        const int pr = get<0>(n);
        const int pc = get<1>(n);
        const int k = get<2>(n);
        const int mr = pr + dr[k];
        const int mc = pc + dc[k];
        if (pr == fr && pc == fc) {
            cout << get<3>(n) << '\n';
            return;
        }
        if (dist[pr][pc][k] >= 0) continue;
        dist[pr][pc][k] = get<3>(n);
        // try moving the shopkeeper
        for (int k2 = 0; k2 < 4; k2++) {
            if (k2 == k) continue;
            const int mr2 = pr + dr[k2];
            const int mc2 = pc + dc[k2];
            if (mr2 >= 0 && mc2 >= 0 && mr2 < R && mc2 < C &&
                grid[mr2][mc2] != 'S' &&
                can_reach(C*mr + mc, C*mr2 + mc2, C*pr + pc)) {
                Q.emplace_front(pr, pc, k2, get<3>(n));
            }
        }
        // try pushing
        const int pr2 = pr - dr[k];
        const int pc2 = pc - dc[k];
        if (pr2 >= 0 && pc2 >= 0 && pr2 < R && pc2 < C &&
            grid[pr2][pc2] != 'S') {
            Q.emplace_back(pr2, pc2, k, get<3>(n) + 1);
        }
    }
    cout << "NO\n";
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
