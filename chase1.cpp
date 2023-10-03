// 2023-10-02
#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <utility>
#include <vector>
using namespace std;
// Find biconnected components reachable from `node`. This is a simplified
// version of the algorithm described on my blog, because it only needs to mark
// vertices, not edges. We don't add back edges to the stack, in order to avoid
// double-counting vertices.
// https://spin0r.wordpress.com/2022/09/11/how-to-find-biconnected-components/
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

void bfs(const vector<vector<int>>& adjlist, int start, vector<int>& dist) {
    queue<pair<int, int>> Q;
    Q.emplace(start, 0);
    while (!Q.empty()) {
        const auto p = Q.front();
        Q.pop();
        if (dist[p.first] <= p.second) continue;
        dist[p.first] = p.second;
        for (const auto neighbor : adjlist[p.first]) {
            Q.emplace(neighbor, p.second + 1);
        }
    }
}

void do_testcase() {
    int V, E;
    cin >> V >> E;
    int a, b; cin >> a >> b; a--; b--;
    vector<vector<int>> adjlist(V);
    while (E--) {
        int u, v; cin >> u >> v; u--; v--;
        adjlist[u].push_back(v);
        adjlist[v].push_back(u);
    }
    vector<vector<int>> bicomp;
    dfs(-1, a, adjlist, bicomp);
    vector<int> adist(V, 1e6);
    bfs(adjlist, a, adist);
    vector<int> bdist(V, 1e6);
    bfs(adjlist, b, bdist);
    // If there's any vertex that's strictly closer to A than to B, *and* that
    // belongs to a biconnected component with strictly more than 2 vertices,
    // then A can always escape by going to that node: due to the triangle-free
    // property of the input, B can't catch A once A reaches such a component.
    for (const auto& bc : bicomp) {
        if (bc.size() <= 2) continue;
        for (const auto v : bc) {
            if (adist[v] < bdist[v]) {
                cout << "No\n";
                return;
            }
        }
    }
    // Otherwise, B can always catch A. To avoid being caught for as long as
    // possible, A should go to the node furthest away from B, but only among
    // nodes that are closer to A than to B in the first place (otherwise B
    // will catch A somewhere along the way).
    int result = 0;
    for (int i = 0; i < V; i++) {
        if (adist[i] < bdist[i]) {
            result = max(result, bdist[i]);
        }
    }
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
