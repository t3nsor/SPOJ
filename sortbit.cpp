// 2024-09-08
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
int nCr[33][33];
int calc(string end, int k) {
    // Return the number of bit-strings of size `end.size()` that are
    // lexicographically less than or equal to `end` and have exactly `k` set
    // bits
    if (k > end.size()) return 0;
    if (end.empty()) return 1;
    int result = 0;
    if (end[0] == '1') {
        result += nCr[end.size() - 1][k];
        k--;
    }
    return result + calc(end.substr(1), k);
}
int calc(string begin, string end, int k) {
    // Return the number of bit-strings of size `begin.size()` that are
    // lexicographically between `begin` and `end` (inclusive) and have exactly
    // `k` set bits
    unsigned int c1 = calc(end, k);
    unsigned int c2 = calc(begin, k);
    unsigned int begin_k = count(begin.begin(), begin.end(), '1') == k;
    return c1 - c2 + begin_k;
}
void do_testcase() {
    int m, n, k; cin >> m >> n >> k; --k;
    unsigned int um = m, un = n;
    if (m < 0 && n <= 0) {
        if (n == 0) {
            // this case is non-continguous so it must be handled separately
            if (k == 0) {
                cout << "0\n";
                return;
            }
            --k;
            un = -1;
        }
    }
    string begin, end;
    for (int i = 31; i >= 0; i--) {
        begin.push_back(m & (1u << i) ? '1' : '0');
        end.push_back(n & (1u << i) ? '1' : '0');
    }
    int bits = 0;
    int cnt;
    while ((cnt = calc(begin, end, bits)) <= k) {
        k -= cnt;
        ++bits;
    }
    string result(32, '0');
    unsigned int u = 0;
    for (int i = 0; i < 32; i++) {
        string s = result;
        for (int j = i + 1; j < 32; j++) s[j] = '1';
        const int cnt = s >= begin ? calc(begin, s, bits) : 0;
        if (cnt <= k) {
            k -= cnt;
            result[i] = '1';
            if (begin < result) begin = result;
        }
        u = (u << 1) + (result[i] - '0');
    }
    cout << int(u) << '\n';
}
int main() {
    for (int n = 0; n <= 32; n++) {
        nCr[n][0] = nCr[n][n] = 1;
        for (int k = 1; k <= n - 1; k++) {
            nCr[n][k] = nCr[n - 1][k] + nCr[n - 1][k - 1];
        }
    }
    int t; cin >> t;
    while (t--) do_testcase();
}
