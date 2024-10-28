// 2024-10-27
#include <algorithm>
#include <ctype.h>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
using namespace std;
struct TrieNode {
    TrieNode* child[26];
    bool isN;
    bool isV;
    bool isA;
};
int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;
    TrieNode root {};
    for (int i = 0; i < N; i++) {
        TrieNode* node = &root;
        string s; cin >> s;
        for (int j = s.size() - 1; j >= 2; j--) {
            if (isalpha(s[j])) {
                if (!node->child[s[j] - 'a']) {
                    node->child[s[j] - 'a'] = new TrieNode();
                }
                node = node->child[s[j] - 'a'];
            }
        }
        if (s[0] == 'n') {
            node->isN = true;
        } else if (s[0] == 'v') {
            node->isV = true;
        } else {
            node->isA = true;
        }
    }
    string line; cin >> line;
    while (!line.empty() && !isalpha(line.back())) line.pop_back();
    if (line.empty()) {
        cout << "0\n0\n"; return 0;
    }
    // 0 = ends with adjective, noun can be next
    // 1 = ends with noun
    // 2 = ends with adjective, verb can be next
    // 3 = ends with verb
    int minsent[5200][4];
    int minword[5200][4];
    memset(minsent, 0x3f, sizeof(minsent));
    for (int i = 1; i <= line.size(); i++) {
        auto improve = [&](int state, int sent, int word) {
            if (sent < minsent[i][state]) {
                minsent[i][state] = sent;
                minword[i][state] = word;
            } else if (sent == minsent[i][state]) {
                minword[i][state] = min(minword[i][state], word);
            }
        };
        TrieNode* node = &root;
        for (int j = 1; j <= 20; j++) {
            node = node->child[line[i - j] - 'a'];
            if (!node) break;
            if (j == i) {
                if (node->isN) {
                    minsent[i][1] = minword[i][1] = 1;
                }
                if (node->isA) {
                    minsent[i][0] = minword[i][0] = 1;
                }
                break;
            }
            if (node->isN) {
                // start new sentence
                improve(1, minsent[i - j][1] + 1, minword[i - j][1] + 1);
                improve(1, minsent[i - j][3] + 1, minword[i - j][3] + 1);
                // add noun to end of existing sentence
                improve(1, minsent[i - j][0], minword[i - j][0] + 1);
                improve(1, minsent[i - j][3], minword[i - j][3] + 1);
            }
            if (node->isV) {
                // add verb to end of existing sentence
                improve(3, minsent[i - j][1], minword[i - j][1] + 1);
                improve(3, minsent[i - j][2], minword[i - j][2] + 1);
            }
            if (node->isA) {
                // start new sentence
                improve(0, minsent[i - j][1] + 1, minword[i - j][1] + 1);
                improve(0, minsent[i - j][3] + 1, minword[i - j][3] + 1);
                // add adjective to end of existing sentence
                improve(0, minsent[i - j][0], minword[i - j][0] + 1);
                improve(0, minsent[i - j][3], minword[i - j][3] + 1);
                improve(2, minsent[i - j][2], minword[i - j][2] + 1);
                improve(2, minsent[i - j][1], minword[i - j][1] + 1);
            }
        }
    }
    int MS = 10000, MW;
    // final state can be only 1 or 3
    for (int i = 1; i <= 3; i += 2) {
        if (minsent[line.size()][i] < MS) {
            MS = minsent[line.size()][i];
            MW = minword[line.size()][i];
        } else if (minsent[line.size()][i] == MS) {
            MW = min(MW, minword[line.size()][i]);
        }
    }
    cout << MS << '\n' << MW << '\n';
}
