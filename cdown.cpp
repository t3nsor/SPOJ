// 2025-06-05
// They don't tell you this in the problem description, but (unlike in real
// life) when a person A has a descendant B, there is a unique d such that B is
// d generations descended from A (the `throw` statement below never runs). This
// makes it easier to form an efficient algorithm. For each person A in the
// tree, we compute a list of values D_{A,0}, ..., D_{A,N-1} where D_{A,B} is
// the number of generations between A and B if B is a descendant of A (and
// minus infinity otherwise); this is easily computed recursively by "merging"
// the lists from A's children.
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
void dfs(const vector<vector<int>>& children, vector<vector<int>>& desc,
         int u) {
    if (!desc[u].empty()) return;
    for (const auto v : children[u]) dfs(children, desc, v);
    desc[u].assign(children.size(), -2000);
    for (const auto v : children[u]) {
        for (int w = 0; w < children.size(); w++) {
            if (desc[v][w] >= 0) {
                if (desc[u][w] < 0) {
                    desc[u][w] = desc[v][w] + 1;
                } else if (desc[u][w] != desc[v][w] + 1) throw;
            }
        }
    }
    desc[u][u] = 0;
}
void do_testcase(int cs) {
    int P, d; cin >> P >> d;
    unordered_map<string, int> id;
    vector<string> name;
    vector<vector<int>> children;
    for (int i = 0; i < P; i++) {
        string parent; cin >> parent;
        auto it = id.find(parent);
        int pid;
        if (it == id.end()) {
            pid = id.size();
            id[parent] = pid;
            name.push_back(parent);
            children.emplace_back();
        } else pid = it->second;
        int nch; cin >> nch;
        while (nch--) {
            int chid;
            string child; cin >> child;
            it = id.find(child);
            if (it == id.end()) {
                chid = id.size();
                id[child] = chid;
                name.push_back(child);
                children.emplace_back();
            } else chid = it->second;
            children[pid].push_back(chid);
        }
    }
    const int V = id.size();
    vector<vector<int>> desc(V);
    for (int i = 0; i < V; i++) {
        dfs(children, desc, i);
    }
    vector<pair<int, string>> ranking;
    for (int i = 0; i < V; i++) {
        ranking.emplace_back(-count(desc[i].begin(), desc[i].end(), d),
                             name[i]);
    }
    sort(ranking.begin(), ranking.end());
    cout << "Tree " << cs << ":\n";
    for (int i = 0; i < ranking.size(); i++) {
        if (ranking[i].first == 0) break;
        if (i >= 3 && ranking[i].first != ranking[2].first) break;
        cout << ranking[i].second << ' ' << -ranking[i].first << '\n';
    }
    cout << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    for (int i = 1; i <= T; i++) do_testcase(i);
}
