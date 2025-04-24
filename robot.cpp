// 2025-04-23
#include <iostream>
using namespace std;
constexpr int MOD = 10000;
int modexp(int p, int e) {
    if (e == 0) return 1;
    int t = modexp(p, e / 2);
    t = (t * t) % MOD;
    if (e % 2) t = (t * p) % MOD;
    return t;
}
void do_testcase() {
    int K; cin >> K;
    int even = 1, odd = 0, prod = 1;
    for (int i = 0; i < K; i++) {
        int p, e; cin >> p >> e;
        prod = (prod * modexp(p, e)) % MOD;
        if (p == 2) continue; else --p;
        const int new_even = (even + p*odd) % MOD;
        const int new_odd = (odd + p*even) % MOD;
        even = new_even;
        odd = new_odd;
    }
    cout << (even + MOD - 1) % MOD << '\n';
    cout << odd << '\n';
    cout << (prod - even - odd + 2*MOD) % MOD << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) do_testcase();
}
