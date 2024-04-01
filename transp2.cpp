// 2024-03-31
// This problem is the same as TRANSP1 except that a + b can now be up to
// 1 million (twice as large) and the number of test cases can now be up to
// 400000. The algorithm is basically the same except that we must be more
// careful not to do too much work per testcase, so:
// * We precompute all relevant powers of 2
// * We precompute all mod inverses using the fact that 2 is a primitive root
//   (this can be verified using a simple test program).
// * We use a faster algorithm to enumerate the divisors.
#include <stdio.h>
#include <vector>
using namespace std;
constexpr int MOD = 1000003;
int factor[1000001];
int phi[1000001];
int modinv[MOD];
int pow2[MOD - 1];
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
void do_testcase() {
    int a, b; scanf("%d %d", &a, &b);
    if (a + b == 0) {
        puts("0");
        return;
    }
    const int g = gcd(a, b);
    const int n = (a + b) / g;
    int sum = 0;
    // enumerate divisors
    int x = n;
    vector<int> divisors = {1};
    while (x > 1) {
        const int f = factor[x];
        int count = 0;
        while (x % f == 0) {
            x /= f;
            count++;
        }
        const int orig_size = divisors.size();
        divisors.resize(orig_size * (count + 1));
        int power = 1;
        for (int i = 1; i <= count; i++) {
            power *= f;
            for (int j = 0; j < orig_size; j++) {
                divisors[j + i * orig_size] = divisors[j] * power;
            }
        }
    }
    for (const int d : divisors) {
        const int exponent = g * d;
        sum = (sum + (pow2[g * d] * (long long)phi[n/d]) % MOD) % MOD;
    }
    const int cycles = (((long long)sum) * modinv[n]) % MOD;
    printf("%d\n", (modexp(2, a + b) - cycles + MOD) % MOD);
}
int main() {
    for (int i = 2; i <= 1000000; i++) {
        if (factor[i]) continue;
        for (int j = i; j <= 1000000; j += i) {
            factor[j] = i;
        }
    }
    phi[1] = 1;
    for (int i = 2; i <= 1000000; i++) {
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
    constexpr long long HALF = (MOD + 1) / 2;
    int power = 1;
    int invpower = 1;
    do {
        modinv[power] = invpower;
        power = (2 * power) % MOD;
        invpower = (HALF * invpower) % MOD;
    } while (power != 1);
    pow2[0] = 1;
    for (int i = 1; i < MOD - 1; i++) {
        pow2[i] = 2 * pow2[i - 1] % MOD;
    }
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
