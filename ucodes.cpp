// 2026-06-21
// This problem is a harder version of AMCODES.  We take a slightly different
// approach here and run a BFS from pairs of terminal nodes (a pair can consist
// of two of the same node, if that node represents a word that appears twice in
// the input) with the root as its destination.  In doing so we record for each
// node a list of its parents in the BFS "tree" (which is actually a DAG) so
// that we can later, starting from the root, reconstruct the lexicographically
// smallest word by taking all 0 edges, if any, or all 1 edges, otherwise.  Note
// that if the answer is 1 bit, you have to print "1 bits" otherwise you will
// get WA.
//
// This solution has a horrendous constant factor and I'm sure better solutions
// exist but I'm too lazy.
#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;
struct Node {
    int child[2] = {-1, -1};
    int cnt = 0;
};
struct Trie {
    vector<Node> nodes;
    Trie() : nodes(1) {}
    void add(const string& word) {
        int cur = 0;
        for (const char c : word) {
            const int x = c - '0';
            if (nodes[cur].child[x] == -1) {
                nodes[cur].child[x] = nodes.size();
                nodes.emplace_back();
            }
            cur = nodes[cur].child[x];
        }
        nodes[cur].cnt++;
    }
};
struct QNode {
    int dist;
    int v;
    int edge;
    int parent;
};
string do_testcase(int m) {
    Trie trie;
    while (m--) { string word; cin >> word; trie.add(word); }
    const int N = trie.nodes.size();
    vector<vector<pair<int, int>>> adj(N*(N+1)/2);
    queue<QNode> Q;
    for (int u = 0; u < trie.nodes.size(); u++) {
        for (int v = u; v < trie.nodes.size(); v++) {
            const int x = u + v*(v + 1)/2;
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j <= min(trie.nodes[u].cnt, 1); j++) {
                    for (int k = 0; k <= min(trie.nodes[v].cnt, 1); k++) {
                        int u2 = (j == 1 ? trie.nodes[0].child[i]
                                         : trie.nodes[u].child[i]);
                        int v2 = (k == 1 ? trie.nodes[0].child[i]
                                         : trie.nodes[v].child[i]);
                        if (u2 >= 0 && v2 >= 0) {
                            if (u2 > v2) swap(u2, v2);
                            const int x2 = u2 + v2*(v2 + 1)/2;
                            // this can introduce duplicates but it doesn't
                            // matter in the end
                            adj[x2].emplace_back(x, i);
                        }
                    }
                }
            }
            if ((u != v && trie.nodes[u].cnt == 1 && trie.nodes[v].cnt == 1) ||
                (u == v && trie.nodes[u].cnt >= 2)) {
                Q.push({0, x, -1, -1});
            }
        }
    }
    vector<int> dist(N*(N+1)/2, 1e6);
    vector<set<pair<int, int>>> parent(N*(N+1)/2);
    while (!Q.empty()) {
        QNode node = Q.front();
        Q.pop();
        if (node.dist > dist[node.v]) continue;
        dist[node.v] = node.dist;
        if (parent[node.v].empty()) {
            for (const auto& e : adj[node.v]) {
                Q.push({node.dist + 1, e.first, e.second, node.v});
            }
        }
        parent[node.v].emplace(node.parent, node.edge);
    }
    string result;
    unordered_set<int> S = {0};
    for (int d = dist[0] - 1; d >= 0; d--) {
        unordered_set<int> S2;
        bool canzero = false;
        for (const int u : S) {
            for (const auto& p : parent[u]) {
                if (p.second == 0) canzero = true;
            }
        }
        result.push_back(canzero ? '0' : '1');
        for (const int u : S) {
            for (const auto& p : parent[u]) {
                if (p.second == 1 && canzero) continue;
                S2.insert(p.first);
            }
        }
        swap(S, S2);
    }
    return result;
}
int main() {
    ios::sync_with_stdio(false);
    for (int i = 1;; i++) {
        int m; cin >> m;
        if (m == 0) break;
        string result = do_testcase(m);
        cout << "Code " << i << ": " << result.size() << " bit";
        if (result.size() > 1) cout << 's';
        cout << '\n';
        for (int j = 0; j < result.size(); j += 20) {
            cout << result.substr(j, 20) << '\n';
        }
        cout << '\n';
    }
}
