// 2023-09-24
//
// This is a DP approach that models building up a palindrome by incrementally
// selecting a word from either the left (i.e., choosing either X[i] or Y[i],
// where `i` is the number of words that have already been taken from the left)
// or the right (analogous). If the concatenation of all words from the left so
// far is longer than the concatenation of all words from the right, we pick the
// next word from the right such that it's consistent with the "overhang" on the
// left, and vice versa. If both substrings are of equal length then we
// arbitrarily pick the next word from the left to avoid overcounting.
//
// In order to compute the transitions efficiently we need to precompute lookup
// tables that tell us whether certain substrings are equal to certain reversed
// substrings. This can be done using the Aho-Corasick algorithm, but a slower
// approach using tries also suffices because the maximum word length is low.
#include <algorithm>
#include <bitset>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
using namespace std;
struct TrieNode {
    vector<int> indices;
    TrieNode* children[26] = {nullptr};
    ~TrieNode() {
        for (int i = 0; i < 26; i++) {
            if (children[i]) delete children[i];
        }
    }
};
struct Trie {
    TrieNode root;
    template <class I>
    TrieNode* insert(int index, I begin, I end, TrieNode* node = nullptr) {
        TrieNode* cur = node ? node : &root;
        while (begin != end) {
            const int c = *begin - 'a';
            if (!cur->children[c]) {
                cur->children[c] = new TrieNode;
            }
            cur = cur->children[c];
            ++begin;
        }
        cur->indices.push_back(index);
        return cur;
    }
    // calls `cb` with the index of the found match
    template <class I, class F>
    void walk(I begin, I end, F cb) {
        TrieNode* cur = &root;
        while (begin != end) {
            int c = *begin - 'a';
            if (!cur->children[c]) return;
            cur = cur->children[c];
            for (const int idx : cur->indices) {
                cb(idx);
            }
            ++begin;
        }
    }
};
void do_testcase() {
    int n; cin >> n;
    vector<string> W(2*n);
    int maxlen = 0;
    for (int i = 0; i < n; i++) {
        cin >> W[2*i];
    }
    for (int i = 0; i < n; i++) {
        cin >> W[2*i+1];
    }
    Trie words;
    Trie rwords;
    for (int i = 0; i < 2*n; i++) {
        maxlen = max<int>(maxlen, W[i].size());
        words.insert(i, W[i].begin(), W[i].end());
        rwords.insert(i, W[i].rbegin(), W[i].rend());
    }
    bitset<64> rmatch[60][400];  // reverse words found here
    bitset<64> match[60][400];  // words found here, looking backward
    for (int i = 0; i < 2*n; i++) {
        // search for substrings of W[i] that are a reverse word
        for (auto J = W[i].begin(); J != W[i].end(); ++J) {
            rwords.walk(J, W[i].end(),
                        [&](int idx) {
                            const int j = J - W[i].begin();
                            rmatch[i][j].set(idx);
                        });
        }
        // search for reverse substrings of W[i] that are a word
        for (auto J = W[i].rbegin(); J != W[i].rend(); ++J) {
            words.walk(J, W[i].rend(),
                       [&](int idx) {
                           const int j = W[i].size() - 1 - (J - W[i].rbegin());
                           match[i][j].set(idx);
                       });
        }
    }
    bitset<64> pmatch[60][400];  // reverse prefix matches of indicated length
    bitset<64> smatch[60][400];  // reverse suffix matches of indicated length
    TrieNode* pnodes[60];
    TrieNode* snodes[60];
    Trie prefixes;
    Trie suffixes;
    fill(pnodes, pnodes + 60, &prefixes.root);
    fill(snodes, snodes + 60, &suffixes.root);
    for (int l = 1; l <= maxlen; l++) {
        for (int i = 0; i < 2*n; i++) {
            pnodes[i]->indices.clear();
            snodes[i]->indices.clear();
        }
        for (int i = 0; i < 2*n; i++) {
            if (W[i].size() < l) continue;
            pnodes[i] = prefixes.insert(i,
                                        W[i].begin() + l - 1,
                                        W[i].begin() + l,
                                        pnodes[i]);
            snodes[i] = suffixes.insert(i,
                                        W[i].rbegin() + l - 1,
                                        W[i].rbegin() + l,
                                        snodes[i]);
        }
        for (int i = 0; i < 2*n; i++) {
            if (W[i].size() < l) continue;
            const auto J = W[i].end() - l;
            const auto rJ = W[i].rend() - l;
            suffixes.walk(J, J + l,
                          [&](int idx) {
                              smatch[i][l].set(idx);
                          });
            prefixes.walk(rJ, rJ + l,
                          [&](int idx) {
                              pmatch[i][l].set(idx);
                          });
        }
    }
    
    // dp state:
    // number of words used from left
    // number of words used from right
    // length difference between left and right, biased by 400
    // set to which the last word from the longer side belongs (
    //     0 for X, 1 for Y, but always 0 if lengths equal
    // )
    int dp[31][31][801][2];
    memset(dp, 0, sizeof(dp));
    dp[0][0][400][0] = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; i + j < n; j++) {
            for (int k = 400 - maxlen; k <= 400 + maxlen; k++) {
                for (int m = 0; m < 2; m++) {
                    if (!dp[i][j][k][m]) continue;
                    if (k > 400) {
                        // The left side is longer.
                        const int diff = k - 400;
                        const int left = 2*(i-1) + m;
                        const int pos = W[left].size() - diff;
                        for (int p = 0; p < 2; p++) {
                            const int right = 2*(n-j-1) + p;
                            if ((W[right].size() < diff &&
                                 rmatch[left][pos][right]) ||
                                (W[right].size() >= diff &&
                                 smatch[left][diff][right])) {
                                const int diff2 = diff - W[right].size();
                                const int newm =
                                  diff2 == 0 ? 0 : (diff2 > 0 ? m : p);
                                dp[i][j + 1][diff2 + 400][newm]
                                  += dp[i][j][k][m];
                            }
                        }
                    } else if (k < 400) {
                        // The right side is longer.
                        const int diff = k - 400;
                        const int right = 2*(n-j) + m;
                        const int pos = (-diff) - 1;
                        for (int p = 0; p < 2; p++) {
                            const int left = 2*i + p;
                            if ((W[left].size() < (-diff) &&
                                 match[right][pos][left]) ||
                                (W[left].size() >= (-diff) &&
                                 pmatch[right][-diff][left])) {
                                const int diff2 = diff + W[left].size();
                                const int newm =
                                  diff2 == 0 ? 0 : (diff2 < 0 ? m : p);
                                dp[i + 1][j][diff2 + 400][newm]
                                  += dp[i][j][k][m];
                            }
                        }
                    } else {
                        // Both sides are equal, so we only take from the left
                        // (to avoid double-counting)
                        for (int p = 0; p < 2; p++) {
                            const int left = 2*i + p;
                            dp[i+1][j][400 + W[left].size()][p] +=
                                dp[i][j][k][m];
                        }
                    }
                }
            }
        }
    }
    // How many palindromes did we get?
    int result = 0;
    for (int i = 0; i <= n; i++) {
        const int j = n - i;
        result += dp[i][j][400][0];  // both sides equal length
        for (int diff = 1; diff <= maxlen; diff++) {
            if (i == 0) break;
            // if the left side is longer, the excess portion of it must be a
            // palindrome by itself
            for (int m = 0; m < 2; m++) {
                if (smatch[2*(i-1)+m][diff][2*(i-1)+m]) {
                    result += dp[i][j][diff + 400][m];
                }
            }
        }
        for (int diff = -1; diff >= -maxlen; diff--) {
            if (j == 0) break;
            for (int m = 0; m < 2; m++) {
                if (pmatch[2*(n-j)+m][-diff][2*(n-j)+m]) {
                    result += dp[i][j][diff + 400][m];
                }
            }
        }
    }
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
