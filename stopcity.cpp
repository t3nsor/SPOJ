// 2022-09-10
#include <cassert>
#include <algorithm>
#include <iostream>
#include <stack>
#include <utility>
#include <vector>
using namespace std;
struct Edge {
    int target;
    size_t back;
    int bicomp;
};
int dfs1(int parent, int node, vector<vector<Edge>>& adjlist) {
    thread_local int current_prenum;
    thread_local int current_bicomp;
    thread_local vector<int> preorder;
    thread_local stack<pair<int, int>> stk;
    if (parent == -1) {
        current_prenum = 0;
        current_bicomp = 0;
        preorder.assign(adjlist.size(), -1);
        assert(stk.empty());
    }

    preorder[node] = current_prenum++;
    int ret = preorder[node];
    int num_children = 0;
    for (size_t i = 0; i < adjlist[node].size(); i++) {
        auto& edge = adjlist[node][i];
        if (edge.target == parent) continue;
        if (preorder[edge.target] >= 0) {
            if (preorder[edge.target] < preorder[node]) {
                ret = min(ret, preorder[edge.target]);
                // this is a back edge that is part of this component
                stk.emplace(node, i);
            }
        } else {
            ++num_children;
            stk.emplace(node, i);
            const int child_ret = dfs1(node, edge.target, adjlist);
            ret = min(ret, child_ret);
            if (parent < 0 ||
                (parent >= 0 && child_ret >= preorder[node])) {
                pair<int, size_t> p;
                do {
                    p = stk.top();
                    auto& fwd = adjlist[p.first][p.second];
                    auto& bwd = adjlist[fwd.target][fwd.back];
                    fwd.bicomp = bwd.bicomp = current_bicomp;
                    stk.pop();
                } while (p != make_pair(node, i));
                ++current_bicomp;
            }
        }
    }
    return ret;
}

void dfs2(int node, pair<int, int> tree_edge,
          const vector<vector<Edge>>& adjlist, vector<pair<int, int>>& prev) {
    prev[node] = tree_edge;
    for (size_t i = 0; i < adjlist[node].size(); i++) {
        const auto& edge = adjlist[node][i];
        if (prev[edge.target].first < 0) {
            dfs2(edge.target, make_pair(node, i), adjlist, prev);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    int V; cin >> V;
    vector<vector<Edge>> adjlist(V);
    for (;;) {
        int v1, v2; cin >> v1 >> v2;
        if (v1 == -1) break;
        adjlist[v1].push_back(Edge{v2, adjlist[v2].size(), -1});
        adjlist[v2].push_back(Edge{v1, adjlist[v1].size() - 1, -1});
    }
    int src, dest; cin >> src >> dest;
    if (src == dest) {
        // edge case: we can't use any edges, although the source node still
        // needs to be printed
        cout << src << '\n'; return 0;
    }

    // 1. Label all biconnected components reachable from `src`
    dfs1(-1, src, adjlist);

    // 2. Find a simple path from src to dest (any path will suffice)
    vector<pair<int, int>> prev(V, make_pair(-1, -1));
    dfs2(src, {-1, -1}, adjlist, prev);

    // 3. Find all edges that are in the same biconnected component as any
    //    node that is on the path. We can visit the endpoints of such edges.
    vector<int> use_component(V, 0);
    do {
        const auto tree_edge = prev[dest];
        use_component[adjlist[tree_edge.first][tree_edge.second].bicomp] = 1;
        dest = tree_edge.first;
    } while (dest != src);

    vector<int> is_visitable(V, 0);
    for (int i = 0; i < V; i++) {
        for (const auto& edge : adjlist[i]) {
            if (edge.bicomp >= 0 && use_component[edge.bicomp]) {
                is_visitable[i] = is_visitable[edge.target] = 1;
            }
        }
    }
    // 4. Print the output
    bool first = true;
    for (int i = 0; i < V; i++) {
        if (!is_visitable[i]) continue;
        if (!first) {
            cout << ' ';
        }
        cout << i;
        first = false;
    }
    cout << '\n';
}
