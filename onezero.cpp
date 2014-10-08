// 2014-05-17
// Idea: BFS from the last digit leftward. This may not actually find the
// smallest answer first, but we can stop going deeper and compare all valid
// answers at the cutoff length.
#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include <vector>
using namespace std;
// done[x] is true iff x consists of only zeroes and ones
bool done[20001];
struct Node {
    char digit; // '0' or '1'
    int parent; // index of parent node in tree, or -1 if root
};
struct State {
    int node;
    int head;
    int length;
};
const int inv[10] = {0, 1, 0, 7, 0, 0, 0, 3, 0, 9};
int main() {
    ios::sync_with_stdio(false);
    for (int i = 0; i <= 20000; i++) {
        ostringstream oss; oss << i;
        done[i] = !~oss.str().find_first_not_of("01");
    }
    int K; cin >> K;
    while (K--) {
        int n; cin >> n;
        int zeroes = 0;
        while (n % 10 == 0) { n /= 10; zeroes++; }
        while (n % 5 == 0) { n /= 5; zeroes++; }
        while (n % 2 == 0) { n /= 2; zeroes++; }
        vector<Node> nodes;
        queue<State> Q;
        int last = inv[n%10];
        Node root; root.digit = '1'; root.parent = -1; nodes.push_back(root);
        State s; s.node = 0; s.head = n*last/10; s.length = 1; Q.push(s);
        int cutoff = 2e9;
        vector<State> final;
        while (!Q.empty()) {
            State s = Q.front(); Q.pop();
            if (s.length > cutoff) continue;
            if (done[s.head]) {
                cutoff = s.length;
                final.push_back(s);
                continue;
            }
            // pick the next digit
            for (int digit = 0; digit <= 1; digit++) {
                int diff = (10 + digit - (s.head % 10))%10;
                int next = (diff * inv[n%10])%10;
                Node node; node.digit = digit + '0'; node.parent = s.node;
                nodes.push_back(node);
                State t; t.node = nodes.size() - 1; t.length = s.length + 1;
                t.head = (s.head + n*next)/10;
                Q.push(t);
            }
        }
        string best;
        for (int i = 0; i < final.size(); i++) {
            ostringstream oss; if (final[i].head) { oss << final[i].head; }
            string s = oss.str();
            int node = final[i].node;
            do {
                s += nodes[node].digit;
                node = nodes[node].parent;
            } while (~node);
            if (i == 0) best = s;
            else if (s.length() < best.length()) best = s;
            else if (s.length() == best.length() && s < best) best = s;
        }
        cout << best;
        while (zeroes--) { cout << '0'; }
        cout << endl;
    }
    return 0;
}
