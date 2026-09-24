// 2026-09-24
// surprisingly annoying problem
// note: it can be assumed that there is always a valid answer (i.e., there is
// no case where the dictionary has only one word and one of the query words is
// identical to the dictionary word).
#include <algorithm>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;
struct Node {
    /// If a dictionary word ends at this node, its index in the sorted list of
    /// dictionary words.
    int term = -1;

    /// The number of dictionary words that use this node.
    int cnt = 0;

    int child[26];

    /// The smallest and second-smallest index of dictionary words that use (not
    /// necessarily end at) this node.
    int i1 = -1, i2 = -1;

    Node() { fill(begin(child), end(child), -1); }
};
int main() {
    char buf[40];
    vector<string> words;
    for (;;) {
        fgets(buf, sizeof(buf), stdin);
        string s;
        for (const char* p = buf; *p > 32; p++) s.push_back(*p);
        if (s.empty()) break; else words.push_back(move(s));
    }
    sort(words.begin(), words.end());
    vector<Node> trie(1);
    for (int i = 0; i < words.size(); i++) {
        int cur = 0;
        for (int j = words[i].size() - 1; j >= 0; j--) {
            const int c = words[i][j] - 'a';
            trie[cur].cnt++;
            if (trie[cur].i1 == -1) trie[cur].i1 = i;
            else if (trie[cur].i2 == -1) trie[cur].i2 = i;
            if (trie[cur].child[c] == -1) {
                trie[cur].child[c] = trie.size();
                trie.emplace_back();
            }
            cur = trie[cur].child[c];
        }
        trie[cur].term = i;
        if (trie[cur].i1 == -1) trie[cur].i1 = i;
        else if (trie[cur].i2 == -1) trie[cur].i2 = i;
    }
    for (;;) {
        if (!fgets(buf, sizeof(buf), stdin)) break;
        int N = 0; while (buf[N] > 32) N++;
        int cur = 0, idx = -1;
        for (int i = N - 1; i >= 0; i--) {
            const int child = trie[cur].child[buf[i] - 'a'];
            cur = child;
            if (child == -1) break;
        }
        if (cur >= 0 && trie[cur].term >= 0) idx = trie[cur].term;
        cur = 0;
        int result = -1;
        for (int i = N - 1; i >= 0; i--) {
            const int child = trie[cur].child[buf[i] - 'a'];
            // can we descend?
            if (child >= 0) {
                if (trie[child].cnt >= 2) {
                    cur = child;
                    continue;
                }
                result = (trie[child].i1 != idx
                          ? trie[child].i1
                          : trie[child].i2);
            } else {
                result = trie[cur].i1;
            }
            break;
        }
        if (result == -1) {
            result = (trie[cur].i1 != idx ? trie[cur].i1 : trie[cur].i2);
        }
        puts(words[result].c_str());
    }
}
