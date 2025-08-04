// 2025-08-03
// This problem is very similar to GEN. The term "consecutive substring" used in
// the problem description just means "substring".
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
constexpr int MOD = 8000;
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
    vector<string> words = {"hl", "hj", "fgd"};
    while (N--) {
        string word; cin >> word;
        if (word == "h" || word == "hl" || word == "hj" || word == "f" ||
            word == "fg" || word == "fgd") {
            cout << "0\n"; return;
        }
        words.push_back(word);
    }
    for (const auto& word : words) {
        for (int i = 0; i < word.size(); i++) {
            nodes.push_back(word.substr(0, i));
        }
    }
    sort(nodes.begin(), nodes.end());
    nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
    // `nodes` consists of all prefixes of words
    vector<vector<int>> matrix(8*nodes.size(), vector<int>(8*nodes.size(), 0));
    for (int from = 0; from < nodes.size(); from++) {
        for (char next = 'a'; next <= 'z'; ++next) {
            string suf = nodes[from];
            bool fail = false;
            for (const auto& word : words) {
                if (word == "hl" || word == "hj" || word == "fgd") continue;
                if (ends_with(suf + next, word)) {
                    fail = true;
                    break;
                }
            }
            if (fail) continue;
            int mask = 0;
            if (ends_with(suf + next, "hl")) {
                mask = 1;
            } else if (ends_with(suf + next, "hj")) {
                mask = 2;
            } else if (ends_with(suf + next, "fgd")) {
                mask = 4;
            }
            int to = -1;
            for (;;) {
                for (int i = 0; i < nodes.size(); i++) {
                    if (suf + next == nodes[i]) {
                        to = i;
                        goto end;
                    }
                }
                if (suf.empty()) {
                    to = 0;
                    goto end;
                } else {
                    suf = suf.substr(1);
                }
            }
end:
            for (int m = 0; m < 8; m++) {
                int m2 = m | mask;
                matrix[8*to + m2][8*from + m]++;
            }
        }
    }
    matrix = modexp(matrix, L);
    int result = 0;
    for (int i = 0; i < nodes.size(); i++) {
        result = (result + matrix[8*i + 7][0]) % MOD;
    }
    cout << result / 8 << '\n';
}
int main() {
    for (;;) {
        int L, N; cin >> L >> N;
        if (cin.eof()) return 0; else do_testcase(N, L);
    }
}
