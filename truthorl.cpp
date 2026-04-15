// 2026-04-14
// According to a comment on the problem description:
// > Output description is wrong. Only output "No inference" if the number of
// > combinations is 0.
// (They got the capitalization wrong, but other than that, the content of that
// comment appears to be correct.)
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
void dfs(int u, const vector<vector<int>>& adj,
         vector<int>& vis, vector<int>& component) {
    if (vis[u]) return;
    vis[u] = 1;
    component.push_back(u);
    for (const auto v : adj[u]) dfs(v, adj, vis, component);
}
void double_bignum(string& s) {
    int carry = 0;
    for (int i = 0; i < s.size(); i++) {
        int digit = s[i] - '0';
        digit = 2*digit + carry;
        carry = digit / 10;
        s[i] = digit % 10 + '0';
    }
    if (carry) s.push_back('1');
}
void do_testcase() {
    int N, M; cin >> N >> M;
    vector<vector<int>> adj(2*M);
    while (N--) {
        int a, b, c, d; cin >> a >> b >> c >> d; --a, --c;
        adj[2*a + b].push_back(2*c + !d);
        adj[2*a + !b].push_back(2*c + d);
        adj[2*c + d].push_back(2*a + !b);
        adj[2*c + !d].push_back(2*a + b);
    }
    vector<vector<int>> component;
    vector<int> vis(2*M, 0);
    vector<int> id(2*M);
    for (int i = 0; i < 2*M; i++) {
        if (!vis[i]) {
            component.emplace_back();
            dfs(i, adj, vis, component.back());
        }
        for (const auto u : component.back()) id[u] = component.size() - 1;
    }
    for (int i = 0; i < M; i++) {
        if (id[2*i] == id[2*i + 1]) {
            cout << "No Inference\n";
            return;
        }
    }
    string result = "1";
    for (int i = 0; i < component.size() / 2; i++) {
        double_bignum(result);
    }
    reverse(result.begin(), result.end());
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T; while (T--) do_testcase();
}
