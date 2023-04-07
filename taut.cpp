// 2023-04-07
#include <algorithm>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;
void do_testcase() {
    string s; cin >> s;
    vector<int> letter_index(256, -1);
    for (int i = 0; i < s.size(); i++) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            letter_index[s[i]] = 0;
        }
    }
    int num_letters = 0;
    for (int i = 0; i < 256; i++) {
        if (letter_index[i] == 0) {
            letter_index[i] = num_letters++;
        }
    }
    bool is_false = false;
    for (int i = 0; i < (1 << num_letters); i++) {
        vector<char> S;
        for (char c : s) {
            S.push_back(c);
            if (S.back() >= 'a' && S.back() <= 'z') {
                S.back() = bool(i & (1 << letter_index[S.back()]));
            }
            for (;;) {
                if (S.size() >= 2 &&
                    S[S.size() - 2] == 'N' &&
                    S[S.size() - 1] <= 1) {
                    int result = !S[S.size() - 1];
                    S.pop_back();
                    S.back() = result;
                } else if (S.size() >= 3 &&
                           S[S.size() - 3] >= 'A' && S[S.size() - 3] <= 'Z' &&
                           S[S.size() - 2] <= 1 &&
                           S[S.size() - 1] <= 1) {
                    const char op = S[S.size() - 3];
                    const int a = S[S.size() - 2];
                    const int b = S[S.size() - 1];
                    S.pop_back();
                    S.pop_back();
                    if (op == 'C') {
                        S.back() = (a && b);
                    } else if (op == 'D') {
                        S.back() = (a || b);
                    } else if (op == 'I') {
                        S.back() = !a || (a && b);
                    } else {
                        S.back() = (a && b) || (!a && !b);
                    }
                } else {
                    break;
                }
            }
        }
        if (S.size() != 1 || S[0] != 1) {
            is_false = true;
            break;
        }
    }
    cout << (is_false ? "NO" : "YES") << '\n';
}
int main() {
    int T; cin >> T;
    while (T--) do_testcase();
}
