// 2025-06-01
// If you play around with the sample input, the rule becomes obvious: the
// output is the reverse of the breadth-first traversal of the expression tree.
#include <algorithm>
#include <ctype.h>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <vector>
using namespace std;
struct Node {
    char op;
    int left;
    int right;
};
string do_testcase(const string& in) {
    string out;
    vector<Node> nodes;
    for (char c = 'a'; c <= 'z'; c++) {
        nodes.push_back({c, -1, -1});
    }
    stack<int> S;
    for (const char c : in) {
        if (islower(c)) {
            S.push(c - 'a');
        } else {
            const int right = S.top();
            S.pop();
            const int left = S.top();
            S.pop();
            nodes.push_back({c, left, right});
            S.push(nodes.size() - 1);
        }
    }
    const int root = S.top();
    queue<int> Q;
    Q.push(root);
    while (!Q.empty()) {
        const int node = Q.front();
        Q.pop();
        out += nodes[node].op;
        if (isupper(nodes[node].op)) {
            Q.push(nodes[node].left);
            Q.push(nodes[node].right);
        }
    }
    reverse(out.begin(), out.end());
    return out;
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) {
        string s; cin >> s;
        cout << do_testcase(s) << '\n';
    }
}
