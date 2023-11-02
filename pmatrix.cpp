// 2023-11-01
// I happened to have a shitty implementation of Gabow's algorithm from the last
// time I attempted this problem, so here it is.
#include <algorithm>
#include <stack>
#include <stdio.h>
#include <vector>
using namespace std;
void scR(int v, const vector<vector<int> >& adj, vector<int>& pre,
         vector<int>& id, int& cnt, int& scnt, stack<int>& S,
         stack<int>& path) {
    int w;
    pre[v] = cnt++;
    S.push(v);
    path.push(v);
    for (int i = 0; i < adj[v].size(); i++) {
        w = adj[v][i];
        if (pre[w] == -1) {
            scR(w, adj, pre, id, cnt, scnt, S, path);
        } else if (id[w] == -1) {
            while (pre[path.top()] > pre[w]) path.pop();
        }
    }
    if (path.top() == v) path.pop(); else return;
    do {
        w = S.top();
        S.pop();
        id[w] = scnt;
    }
    while (w != v);
    scnt++;
}
void do_testcase() {
    int V, E; scanf("%d %d", &V, &E);
    vector<int> pre(V, -1);
    vector<int> id(V, -1);
    vector<vector<int> > adj(V);
    int scnt = 0;
    int cnt = 0;
    while (E--) {
        int u, v; scanf("%d %d", &u, &v);
        adj[u - 1].push_back(v - 1);
    }
    for (int i = 0; i < V; i++) {
        stack<int> S;
        stack<int> path;
        if (id[i] == -1) scR(i, adj, pre, id, cnt, scnt, S, path);
    }
    vector<int> in(scnt);
    vector<int> out(scnt);
    for (int u = 0; u < V; u++) {
        for (int j = 0; j < adj[u].size(); j++) {
            const int v = adj[u][j];
            const int scc1 = id[u];
            const int scc2 = id[v];
            if (scc1 != scc2) {
                ++out[scc1];
                ++in[scc2];
            }
        }
    }
    int sources = 0;
    int sinks = 0;
    for (int i = 0; i < scnt; i++) {
        if (in[i] == 0) ++sources;
        if (out[i] == 0) ++sinks;
    }
    if (scnt == 1) {
        puts("0");
    } else {
        printf("%d\n", max(sources, sinks));
    }
}
int main() {
    int T; scanf("%d", &T);
    while (T--) do_testcase();
}
