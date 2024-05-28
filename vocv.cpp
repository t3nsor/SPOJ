// 2024-05-27
#include <algorithm>
#include <stdio.h>
#include <vector>
using namespace std;
const int MOD = 10007;
void dfs(int parent, int cur, const vector<vector<int>>& adjlist,
         vector<int>& opt0, vector<int>& opt1,
         vector<int>& ways0, vector<int>& ways1) {
    opt0[cur] = 0;
    opt1[cur] = 1;
    ways0[cur] = ways1[cur] = 1;
    for (int child : adjlist[cur]) {
        if (child == parent) continue;
        dfs(cur, child, adjlist, opt0, opt1, ways0, ways1);
        opt0[cur] += opt1[child];
        ways0[cur] = (ways0[cur] * ways1[child]) % MOD;
        int childways = 0;
        const int childopt = min(opt0[child], opt1[child]);
        opt1[cur] += childopt;
        if (opt0[child] == childopt) {
            childways += ways0[child];
        }
        if (opt1[child] == childopt) {
            childways += ways1[child];
        }
        childways %= MOD;
        ways1[cur] = (ways1[cur] * childways) % MOD;
    }
}
void do_testcase() {
    int V; scanf("%d", &V);
    vector<vector<int>> adjlist(V);
    for (int i = 0; i < V - 1; i++) {
        int u, v; scanf("%d %d", &u, &v); --u; --v;
        adjlist[u].push_back(v);
        adjlist[v].push_back(u);
    }
    vector<int> opt0(V), opt1(V), ways0(V), ways1(V);
    dfs(-1, 0, adjlist, opt0, opt1, ways0, ways1);
    if (opt0[0] < opt1[0]) {
        printf("%d %d\n", opt0[0], ways0[0]);
    } else if (opt1[0] < opt0[0]) {
        printf("%d %d\n", opt1[0], ways1[0]);
    } else {
        printf("%d %d\n", opt0[0], (ways0[0] + ways1[0]) % MOD);
    }
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
