// 2023-10-14
// Since each pair of consecutive soldiers must have ranks differing by at
// least 2, it follows that if the first soldier has rank 1 or 2, then the
// second soldier must have rank 3 or 4, the third must have 1 or 2, the fourth
// must have 3 or 4, and so on. We can in fact assume that the first soldier
// has rank either 1 or 2 without loss of generality. Imposing the additional
// rules from the province then turns this into a 2-SAT problem.
#include <algorithm>
#include <stdio.h>
#include <vector>
using namespace std;
void dfs1(const vector<vector<int>>& adj, vector<int>& post,
          int& cur_post, int v) {
    if (post[v] >= 0) return;
    post[v] = 0;  // mark as visited, we will fix it later
    for (const int w : adj[v]) {
        dfs1(adj, post, cur_post, w);
    }
    post[v] = cur_post++;
}
void dfs2(const vector<vector<int>>& adj, vector<int>& scc_id,
          int scc_cnt, int v) {
    scc_id[v] = scc_cnt;
    for (const int w : adj[v]) {
        if (scc_id[w] < 0) {
            dfs2(adj, scc_id, scc_cnt, w);
        }
    }
}
bool do_testcase() {
    int n, p; scanf("%d %d", &n, &p);
    vector<vector<int>> rules(p);
    bool fail = false;
    for (int i = 0; i < p; i++) {
        int m; scanf("%d", &m);
        if (m > 4) fail = true;
        while (m--) {
            int x; scanf("%d", &x); rules[i].push_back(x-1);
        }
        sort(rules[i].begin(), rules[i].end());
        if (unique(rules[i].begin(), rules[i].end()) != rules[i].end()) {
            fail = true;
        }
    }
    if (n == 0) return true;
    if (fail) return false;
    // node IDs are as follows
    // 4k ->     rank 1 at index 2k
    // 4k + 1 -> rank 2 at index 2k
    // 4k + 2 -> rank 3 at index 2k + 1
    // 4k + 3 -> rank 4 at index 2k + 1
    vector<vector<int>> adj(2*n);
    for (int i = 0; i + 1 < n; i++) {
        const int k = i / 2;
        if (i % 2 == 0) {
            // we can be 1 or 2; if we are 2, then the next guy must be 4
            adj[4*k+1].push_back(4*k+3);
            // if the next guy is 3, then we must be 1
            adj[4*k+2].push_back(4*k);
        } else {
            // we can be 3 or 4; if we are 3, then the next guy must be 1
            adj[4*k+2].push_back(4*k+4);
            // if the next guy is 2, then we must be 4
            adj[4*k+5].push_back(4*k+3);
        }
    }
    // add rules
    for (const auto& rule : rules) {
        for (int i = 0; i < rule.size(); i++) {
            for (int j = 0; j < i; j++) {
                if (rule[i] % 2 != rule[j] % 2) {
                    // they will always be different already
                    continue;
                }
                const int k = rule[i] / 2;
                const int m = rule[j] / 2;
                if (rule[i] % 2 == 0) {
                    // if we are 1 then the other guy must be 2 and vice versa
                    adj[4*k].push_back(4*m+1);
                    adj[4*k+1].push_back(4*m);
                    adj[4*m].push_back(4*k+1);
                    adj[4*m+1].push_back(4*k);
                } else {
                    // if we are 3 then the other guy must be 4 and vice versa
                    adj[4*k+2].push_back(4*m+3);
                    adj[4*k+3].push_back(4*m+2);
                    adj[4*m+2].push_back(4*k+3);
                    adj[4*m+3].push_back(4*k+2);
                }
            }
        }
    }
    vector<int> post(2*n, -1);
    int cur_post = 0;
    for (int i = 0; i < 2*n; i++) {
        dfs1(adj, post, cur_post, i);
    }
    vector<int> rpost(2*n);
    for (int i = 0; i < 2*n; i++) {
        rpost[post[i]] = i;
    }
    // reverse all arcs
    {
        vector<vector<int>> adj2(2*n);
        for (int i = 0; i < adj.size(); i++) {
            for (const int v : adj[i]) {
                adj2[v].push_back(i);
            }
        }
        adj = move(adj2);
    }
    vector<int> scc_id(2*n, -1);
    int scc_cnt = 0;
    for (int i = 2*n - 1; i >= 0; i--) {
        const int v = rpost[i];
        if (scc_id[v] == -1) {
            dfs2(adj, scc_id, scc_cnt, v);
        }
        ++scc_cnt;
    }
    for (int i = 0; i < n; i++) {
        if (scc_id[2*i] == scc_id[2*i+1]) {
            return false;
        }
    }
    return true;
}
int main() {
    int t; scanf("%d", &t);
    while (t--) {
        puts(do_testcase() ? "approved" : "rejected");
    }
}
