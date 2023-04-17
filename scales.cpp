// 2023-04-17
// Basically, we have to count the number of bitstrings of length `N` such that,
// letting `x` be the value of the bitstring, x + W is less than 2^N and has no
// bits in common with `x`. This can be done using dynamic programming, where
// dp0[i] is the number of ways of selecting the last `i` bits of a bitstring
// such that when added to the last `i` bits of `x`, the result has no bits in
// common with the original bitstring, and the addition does not carry (i.e.,
// exceed 2^i), and dp1[i] is the same except that the addition does carry.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
void do_testcase() {
    int N, L, D; cin >> N >> L >> D;
    string W; W.reserve(L); cin >> W;
    vector<int> dp0(N + 1);
    vector<int> dp1(N + 1);
    dp0[0] = 1;
    dp1[0] = 0;
    for (int i = 1; i <= N; i++) {
        char c;
        if (i <= L) {
            c = W[L - i];
        } else {
            c = '0';
        }
        if (c == '0') {
            dp0[i] = (dp0[i - 1] + dp1[i - 1]) % D;
            dp1[i] = dp1[i - 1];
        } else {
            dp0[i] = dp0[i - 1];
            dp1[i] = (dp0[i - 1] + dp1[i - 1]) % D;
        }
    }
    cout << dp0[N] << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
