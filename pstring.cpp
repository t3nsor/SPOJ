// 2024-08-20
#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;
void do_testcase(const string& X, const string& Y) {
    for (char c : X) {
        if (c < 'a' || c > 'z') throw;
    }
    for (char c : Y) {
        if (c < 'a' || c > 'z') throw;
    }
    // compute KMP prefix function
    vector<int> p(Y.size() + 1);
    p[0] = -1;
    for (int i = 0; i < Y.size(); i++) {
        int j = i;
        while (j > 0 && Y[i] != Y[p[j]]) {
            j = p[j];
        }
        p[i + 1] = p[j] + 1;
    }
    // compute table `t`, where t[i][j] is the longest suffix of Y[0 ... i - 1]
    // + j that is a prefix of `Y`
    vector<array<int, 26>> t(Y.size());
    for (int i = 0; i < Y.size(); i++) {
        for (int j = 0; j < 26; j++) {
            const char c = j + 'a';
            int k = i;
            while (k >= 0 && c != Y[k]) {
                k = p[k];
            }
            t[i][j] = k + 1;
        }
    }
    // main algorithm
    // dp[l] is the minimum number of characters to remove such that `l` is the
    // longest prefix that is a suffix of the resulting string
    vector<int> dp(Y.size(), 1e6);
    dp[0] = 0;
    for (char c : X) {
        vector<int> dp2(Y.size(), 1e6);
        // reject the current character
        for (int i = 0; i < Y.size(); i++) {
            dp2[i] = dp[i] + 1;
        }
        // accept the current character
        for (int i = 0; i < Y.size(); i++) {
            if (t[i][c - 'a'] < Y.size()) {
                dp2[t[i][c - 'a']] = min(dp2[t[i][c - 'a']], dp[i]);
            }
        }
        swap(dp, dp2);
    }
    cout << *min_element(dp.begin(), dp.end()) << '\n';
}
int main() {
    string X, Y;
    while (cin >> X >> Y) {
        do_testcase(X, Y);
    }
}
