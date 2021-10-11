// 2021-10-11
#include <iostream>
#include <stack>
#include <utility>
#include <vector>
using namespace std;
void dfs(int v, const vector<vector<int>>& adj, vector<int>& ccomp, int n) {
    ccomp[v] = n;
    for (const auto u : adj[v]) {
        if (ccomp[u] < 0) {
            dfs(u, adj, ccomp, n);
        }
    }
}
void do_testcase() {
    int V, E; cin >> V >> E;
    vector<vector<int>> ladj(V);
    vector<vector<int>> eadj(V);
    while (E--) {
        int v1, v2, r; cin >> v1 >> v2 >> r;
        v1--; v2--;
        if (r == 0) {
            eadj[v1].push_back(v2);
            eadj[v2].push_back(v1);
            continue;
        }
        if (r > 0) swap(v1, v2);
        ladj[v1].push_back(v2);
    }
    vector<int> ccomp(V, -1);
    int nccomp = 0;
    for (int i = 0; i < V; i++) {
        if (ccomp[i] < 0) {
            dfs(i, eadj, ccomp, nccomp);
            ++nccomp;
        }
    }
    vector<int> indeg(nccomp);
    vector<int> minval(nccomp, 0);
    int result = 0;
    int vis = 0;
    {
        vector<vector<int>> ladj2(nccomp);
        for (int i = 0; i < V; i++) {
            for (const auto j : ladj[i]) {
                ladj2[ccomp[i]].push_back(ccomp[j]);
                ++indeg[ccomp[j]];
            }
        }
        swap(ladj, ladj2);
        V = nccomp;
    }
    stack<int> S;
    for (int i = 0; i < V; i++) {
        if (indeg[i] == 0) S.push(i);
    }
    while (!S.empty()) {
        const int v = S.top(); S.pop();
        ++vis;
        result = max(result, minval[v]);
        for (const auto u : ladj[v]) {
            minval[u] = max(minval[u], minval[v] + 1);
            if (0 == --indeg[u]) {
                S.push(u);
            }
        }
    }
    if (vis < V) {
        cout << "NO";
    } else {
        cout << result;
    }
    cout << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int d; cin >> d;
    while (d--) {
        do_testcase();
    }
}
