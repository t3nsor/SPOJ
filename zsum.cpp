// 2014-04-30
#include <iostream>
using namespace std;
const long long MOD = 10000007;
long long modexp(long long base, long long exponent) {
    long long pwr = base;
    long long res = 1;
    while (exponent) {
        if (exponent & 1) {
            res = (res * pwr) % MOD;    
        }
        exponent >>= 1;
        pwr = (pwr * pwr) % MOD;
    }
    return res;
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        int n, k; cin >> n >> k; if (n == 0) return 0;
        long long res = modexp(n, k) + 2*modexp(n-1, k)
                      + modexp(n, n) + 2*modexp(n-1, n-1);
        cout << res % MOD << endl;
    }
}
