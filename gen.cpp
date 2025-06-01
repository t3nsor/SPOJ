// 2025-05-31
// The problem asks us to calculate the number of strings of length L,
// consisting of uppercase letters, having at least one of the given strings as
// a substring. We approach this by calculating the number of strings that have
// none of the given strings as a substring. This can be viewed as the number of
// paths through the Aho-Corasick automaton for the set of strings that avoid
// all dictionary nodes. Because there are so few strings and they're short, we
// can construct the automaton using a naive algorithm, and we can skip
// generating the dictionary links because we want to avoid matches, not find
// them. To find the total number of paths we have to use matrix
// exponentiation. Minor constant optimization is necessary: we should declare
// the modulus as a global constant so the compiler can generate slightly
// faster code for the division. A second constant optimization (not necessary
// for getting AC) is to use a `long long` accumulator in the inner loop so
// that we only need to mod once at the end.
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
constexpr int MOD = 10007;
bool ends_with(const string& s1, const string& s2) {
    if (s1.length() < s2.length()) return false;
    return s1.substr(s1.length() - s2.length()) == s2;
}
vector<vector<int>> modmul(const vector<vector<int>>& m1,
                           const vector<vector<int>>& m2) {
    const int N = m1.size();
    vector<vector<int>> result(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            long long x = 0;
            for (int k = 0; k < N; k++) {
                x += m1[i][k] * m2[k][j];
            }
            result[i][j] = x % MOD;
        }
    }
    return result;
}
vector<vector<int>> modexp(const vector<vector<int>>& m, int e) {
    if (e == 1) return m;
    auto result = modexp(m, e / 2);
    result = modmul(result, result);
    if (e % 2) result = modmul(m, result);
    return result;
}
int modexp(int a, int e) {
    if (e == 1) return a;
    int result = modexp(a, e / 2);
    result = (result * result) % MOD;
    if (e % 2) result = (result * a) % MOD;
    return result;
}
void do_testcase(int N, int L) {
    vector<string> nodes;
    vector<string> words;
    while (N--) {
        string word; cin >> word; words.push_back(word);
        for (int i = 0; i < word.size(); i++) {
            nodes.push_back(word.substr(0, i));
        }
    }
    for (int i = 0; i < nodes.size();) {
        bool remove = false;
        for (const auto& word : words) {
            if (nodes[i].find(word) == 0) {
                remove = true;
                break;
            }
        }
        if (remove) {
            nodes.erase(nodes.begin() + i);
        } else {
            i++;
        }
    }
    sort(nodes.begin(), nodes.end());
    nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
    // `nodes` consists of all prefixes of words, except those that have a word
    // as a prefix
    vector<vector<int>> matrix(nodes.size(), vector<int>(nodes.size(), 0));
    for (int from = 0; from < nodes.size(); from++) {
        for (char next = 'A'; next <= 'Z'; ++next) {
            string suf = nodes[from];
            bool fail = false;
            for (const auto& word : words) {
                if (ends_with(suf + next, word)) {
                    fail = true;
                    break;
                }
            }
            if (fail) continue;
            for (;;) {
                int to;
                for (to = 0; to < nodes.size(); to++) {
                    if (suf + next == nodes[to]) break;
                }
                if (to < nodes.size()) {
                    matrix[to][from]++;
                    break;
                } else if (!suf.empty()) {
                    suf = suf.substr(1);
                } else {
                    matrix[0][from]++;
                    break;
                }
            }
        }
    }
    matrix = modexp(matrix, L);
    int result = modexp(26, L);
    for (int i = 0; i < nodes.size(); i++) {
        result = (result - matrix[i][0] + MOD) % MOD;
    }
    cout << result << '\n';
}
int main() {
    for (;;) {
        int N, L; cin >> N >> L;
        if (cin.eof()) return 0; else do_testcase(N, L);
    }
}
