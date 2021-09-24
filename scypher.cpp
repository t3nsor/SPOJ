// 2021-09-24
#include <algorithm>
#include <cctype>
#include <iostream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
using namespace std;
// counts number of nodes reachable from given node (including itself)
int dfs(int v, const vector<vector<int>>& adj) {
    const auto N = adj.size();
    vector<int> vis(N, 0);
    stack<int> stk;
    stk.push(v);
    int result = 0;
    while (!stk.empty()) {
        const auto u = stk.top();
        stk.pop();
        if (vis[u]) continue;
        vis[u] = 1;
        ++result;
        for (const auto w : adj[u]) {
            stk.push(w);
        }
    }
    return result;
}
void do_testcase() {
    int A, K; cin >> A >> K;
    string previous_word;
    vector<vector<int>> adj(A);
    vector<vector<int>> radj(A);
    for (int i = 0; i < K; i++) {
        string word; cin >> word;
        for (int j = 0; j < min(word.size(), previous_word.size()); j++) {
            if (previous_word[j] != word[j]) {
                adj[previous_word[j] - 'a'].push_back(word[j] - 'a');
                break;
            }
        }
        swap(word, previous_word);
    }
    for (int i = 0; i < A; i++) {
        for (const auto neighbour : adj[i]) {
            radj[neighbour].push_back(i);
        }
    }
    // Assign positions
    vector<int> pos(A, -1);
    for (int i = 0; i < A; i++) {
        const int before = dfs(i, radj);
        const int after = dfs(i, adj);
        if (before + after == A + 1) {
            pos[i] = before - 1;
        }
    }
    // Read the message (annoying)
    char c;
    do {
        c = cin.get();
    } while (isspace(c));
    string message;
    do {
        message.push_back(c);
        c = cin.get();
    } while (c != '\n');
    // Decode
    bool success = true;
    for (auto& c : message) {
        if (c < 'a' || c >= ('a' + A)) continue;
        if (pos[c - 'a'] == -1) {
            success = false;
            break;
        }
        c = pos[c - 'a'] + 'a';
    }
    if (success) {
        cout << message << '\n';
    } else {
        cout << "Message cannot be decrypted.\n";
    }
}
int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;
    while (N--) {
        do_testcase();
    }
}
