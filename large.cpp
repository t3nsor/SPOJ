// 2026-09-28
// It can be assumed that we will use Burnside's lemma.  Dividing by N is easy
// since the modulus is 10^8 + 7 (which, like its more common cousin 10^9 + 7,
// is prime).  Let L(K, N) denote the number of bitstrings of length N with no
// more than K consecutive ones, where the first and last bit are considered
// adjacent.  For the purposes of L(K, N), a bit can be used multiple times in a
// group, so L(K, 1), ..., L(K, K) exclude the bitstring of all 1's.
//
// If N <= K, then the number of valid arrangements fixed by the rotation by M
// bits (0 <= M < N) is 2^{gcd(N, M)}.  Otherwise, it's L(K, gcd(N, M)).  The
// nontrivial part of the problem that remains is to compute the table L(K, N)
// where 1 <= K, N <= 999, in approximately constant time per entry.
//
// We always have L(K, N) = 2^N - 1 when 1 <= N <= K + 1.  We will prove that
// for N > K + 1, L(K, N) = L(K, N - K - 1) + ... + L(K, N - 1).
//
// L(K, N) can be viewed as the number of ways of filling N squares arranged in
// a circle with rectangles of lengths 1 x 1, ..., 1 x (K + 1), representing
// the substrings 0, 10, ..., {1^K}0.  To form a bijection from such
// arrangements to the union of the sets of valid arrangements with N - 1, ...,
// N - K - 1 squares, simply remove the rectangle that ends latest in the string
// (a rectangle that covers both the first and last positions is never removed).

#include <algorithm>
#include <iostream>
using namespace std;
using LL = long long;
constexpr int MOD = 100000007;

int inv[1000];
int pow2[1000];

int gcd(int x, int y) { if (x == 0) return y; else return gcd(y % x, x); }

int modexp(int b, int e) {
    if (e == 0) return 1;
    LL r = modexp(b, e / 2);
    r = (r * r) % MOD;
    if (e & 1) r = (r * b) % MOD;
    return r;
}

int L[1000][1000];  // K, N

int main() {
    ios::sync_with_stdio(false);
    for (int i = 1; i <= 999; i++) inv[i] = modexp(i, MOD - 2);
    pow2[0] = 1;
    for (int i = 1; i <= 999; i++) pow2[i] = (2*pow2[i - 1]) % MOD;
    for (int K = 0; K <= 999; K++) {
        int sum = 0;
        for (int N = 1; N <= min(K + 1, 999); N++) {
            L[K][N] = (pow2[N] + MOD - 1) % MOD;
            sum = (sum + L[K][N]) % MOD;
        }
        for (int N = K + 2; N <= 999; N++) {
            L[K][N] = sum;
            sum = (sum + L[K][N] - L[K][N - K - 1] + MOD) % MOD;
        }
    }
    int T; cin >> T;
    while (T--) {
        int N, K; cin >> N >> K;
        if (K > N) K = N;
        int result = 0;
        for (int M = 0; M < N; M++) {
            const auto g = gcd(M, N);
            if (N == K) {
                result = (result + pow2[g]) % MOD;
            } else {
                result = (result + L[K][g]) % MOD;
            }
        }
        cout << (LL(result) * inv[N]) % MOD << '\n';
    }
}
