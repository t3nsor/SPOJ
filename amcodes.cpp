// 2026-05-19
// To find the shortest ambiguous message, we use BFS where each node
// represents a pair of "states" that are valid for a particular string.  A
// state is a node in the trie of all valid words, which is reached by starting
// from the root and walking down the trie based on a given string, except that
// when we reach a terminal node we are allowed to continue from the root.
// When we reach a pair of states that are both terminal, it means the string
// is ambiguous.  Once we run out of pairs of states, we give up.
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;
struct Node {
    int child[16];
    bool term = false;
    Node() { for (int i = 0; i < 16; i++) child[i] = -1; }
};
struct Trie {
    vector<Node> nodes;
    Trie() : nodes(1) {}
    void add(const string& word) {
        int node = 0;
        for (int i = 0; i < word.size(); i++) {
            const int val = (word[i] <= '9'
                             ? word[i] - '0'
                             : word[i] - 'A' + 10);
            if (nodes[node].child[val] == -1) {
                nodes[node].child[val] = nodes.size();
                nodes.emplace_back();
            }
            node = nodes[node].child[val];
        }
        nodes[node].term = true;
    }
};
struct QNode {
    int d;
    int u;
    int v;
};
int do_testcase(int N) {
    Trie trie;
    while (N--) { string word; cin >> word; trie.add(word); }
    vector<vector<char>> vis(trie.nodes.size(),
                             vector<char>(trie.nodes.size(), 0));
    queue<QNode> Q;
    Q.push({0, 0, 0});
    while (!Q.empty()) {
        QNode p = Q.front();
        Q.pop();
        if (vis[p.u][p.v]) continue;
        vis[p.u][p.v] = 1;
        if (trie.nodes[p.u].term && trie.nodes[p.v].term && p.u != p.v) {
            return p.d;
        }
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j <= trie.nodes[p.u].term; j++) {
                for (int k = 0; k <= trie.nodes[p.v].term; k++) {
                    const int u2 = (j == 1
                                    ? trie.nodes[0].child[i]
                                    : trie.nodes[p.u].child[i]);
                    const int v2 = (k == 1
                                    ? trie.nodes[0].child[i]
                                    : trie.nodes[p.v].child[i]);
                    if (u2 >= 0 && v2 >= 0) {
                        Q.push({p.d + 1, min(u2, v2), max(u2, v2)});
                    }
                }
            }
        }
    }
    return -1;
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        int N; cin >> N; if (N == 0) break;
        cout << do_testcase(N) << '\n';
    }
}
