// 2024-03-10
#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;
void do_testcase() {
    vector<vector<int>> parents(100);
    vector<vector<int>> children(100);
    vector<char> operation(100);
    vector<int> is_input(100, 0);
    vector<int> is_output(100, 0);
    vector<int> inputs;
    vector<int> outputs;
    for (;;) {
        string cell; cin >> cell;
        if (cell == "end") break;
        const int node = 10 * (cell[0] - '0') + (cell[1] - '0');
        operation[node] = cell[2];
        if (operation[node] == 'i' && !is_input[node]) {
            is_input[node] = 1;
            inputs.push_back(node);
        }
        if (operation[node] == 'o' && !is_output[node]) {
            is_output[node] = 1;
            outputs.push_back(node);
        }
        for (;;) {
            string child; cin >> child;
            if (child == "..") break;
            const int child_node = 10 * (child[0] - '0') + (child[1] - '0');
            children[node].push_back(child_node);
            parents[child_node].push_back(node);
        }
    }
    int t; cin >> t;
    while (t--) {
        string values; cin >> values;
        vector<int> indeg(100);
        vector<int> value(100);
        for (int i = 0; i < 100; i++) indeg[i] = parents[i].size();
        queue<int> Q;
        for (int i = 0; i < inputs.size(); i++) {
            const int node = inputs[i];
            value[node] = values[i] - '0';
            for (const int child : children[node]) {
                if (0 == --indeg[child]) {
                    Q.push(child);
                }
            }
        }
        while (!Q.empty()) {
            const int node = Q.front();
            Q.pop();
            if (operation[node] == '&') {
                value[node] = value[parents[node][0]] & value[parents[node][1]];
            } else if (operation[node] == '|') {
                value[node] = value[parents[node][0]] | value[parents[node][1]];
            } else if (operation[node] == '!') {
                value[node] = !value[parents[node][0]];
            } else {
                // operation[node] == 'o'
                value[node] = value[parents[node][0]];
            }
            for (const int child : children[node]) {
                if (0 == --indeg[child]) {
                    Q.push(child);
                }
            }
        }
        for (const int output : outputs) {
            cout << char('0' + value[output]);
        }
        cout << '\n';
    }
}
int main() {
    ios::sync_with_stdio(false);
    int n; cin >> n;
    while (n--) {
        do_testcase();
        cout << '\n';
    }
}
