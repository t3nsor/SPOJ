// 2024-03-31
// A position within the array is labelled by an (a + b)-bit string, and the
// transposition operation corresponds to a (left or right) rotation by `a`
// positions. Since a cycle of length k requires k-1 swaps, the total number of
// swaps required is just 2^(a+b) minus the number of cycles. To determine the
// number of cycles we use the Pólya enumeration theorem. We also use the fact
// that
//   \sum_{i=1}^n m^gcd(i, n) = \sum_{d|n} m^d |{i : gcd(i, n) = d}|
//                            = \sum_{d|n} m^d phi(n/d)
#include <iostream>
using namespace std;
constexpr int MOD = 1000003;
int factor[500001];
int phi[500001];
int gcd(int x, int y) {
    if (x == 0) return y;
    return gcd(y%x, x);
}
int modexp(int b, int e) {
    if (e == 0) return 1;
    long long result = modexp(b, e / 2);
    result = (result * result) % MOD;
    if (e % 2) {
        result = (result * b) % MOD;
    }
    return result;
}
int modinv(int x) {
    return modexp(x, MOD - 2);
}
void do_testcase() {
    int a, b; cin >> a >> b;
    if (a + b == 0) {
        cout << "0\n";
        return ;
    }
    const int g = gcd(a, b);
    const int n = (a + b) / g;
    const int m = modexp(2, g);
    int sum = 0;
    for (int d = 1; d*d <= n; d++) {
        if (n % d) continue;
        if (d*d == n) {
            const long long power = modexp(m, d);
            sum = (sum + (power * phi[n/d]) % MOD) % MOD;
        } else {
            const long long power1 = modexp(m, d);
            const long long power2 = modexp(m, n/d);
            sum = (sum + (power1 * phi[n/d]) % MOD) % MOD;
            sum = (sum + (power2 * phi[d]) % MOD) % MOD;
        }
    }
    const int cycles = (((long long)sum) * modinv(n)) % MOD;
    cout << (modexp(2, a + b) - cycles + MOD) % MOD << '\n';
}
int main() {
    for (int i = 2; i <= 500000; i++) {
        if (factor[i]) continue;
        for (int j = i; j <= 500000; j += i) {
            factor[j] = i;
        }
    }
    phi[1] = 1;
    for (int i = 2; i <= 500000; i++) {
        phi[i] = 1;
        int j = i;
        while (j > 1) {
            const int k = factor[j];
            phi[i] *= k - 1;
            j /= k;
            while (j % k == 0) {
                j /= k;
                phi[i] *= k;
            }
        }
    }
    int t; cin >> t;
    while (t--) do_testcase();
}
