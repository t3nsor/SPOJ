// 2025-06-15
//
// The starting point for this problem is DETER/MSE08H. For DETER, the answer is
// phi(1) * ... * phi(n). For MSE08H, the answer is likewise the product of
// phi(x_i) for all x_i in the input.
//
// Once the proof is known, generalizing the formula to solve DETER2 is not that
// hard. (I'm not sure how you'd find the proof in a contest environment,
// though.) Here's the proof. If {x_1, ..., x_n} is a factor-closed set, then
// the gcd matrix can be written as A^T A where
//
// A_{ij} = sqrt(phi(x_i)), if x_i divides x_j
//          0             , otherwise
//
// Note that A is upper triangular, so det(A^T A) is the product of the squares
// of the diagonal entries.
//
// To prove the factorization, define M = A^T A; then
// M_{ij} = \sum_k A_{ki} A_{kj}
//        = \sum_{x_k|x_i and x_k|x_j} sqrt(phi(x_k)) sqrt(phi(x_k))
//        = \sum_{x_k|gcd(x_i, x_j)} phi(x_k)
//        = gcd(x_i, x_j)
//
// Note that the k above is a dummy index, but below k will be the exponent from
// the problem statement.
//
// The above result can be generalized to make the right side be equal to
// gcd(x_i, x_j)^k if we set A_{ij} = sqrt(f_k(x_i)) (when x_i divides x_j)
// where f_k is the function that satisfies n^k = \sum_{d|n} f_k(d). The case of
// k = 1 is the well-known identity n = \sum_{d|n} phi(d).
//
// To find f_k, we use the Mobius inversion formula. This is left as an exercise
// for the reader.
#include <iostream>
#include <vector>
using namespace std;
constexpr int MOD = 1000003;
int factor[1000001];
int modmul(int x, int y) {
    return (x * (long long)y) % MOD;
}
int modexp(int b, int e) {
    if (e == 1) return b;
    int result = modexp(b, e / 2);
    result = modmul(result, result);
    if (e & 1) result = modmul(result, b);
    return result;
}
void do_testcase() {
    int n, k; cin >> n >> k;
    vector<int> f(n + 1);
    f[1] = 1;
    for (int i = 2; i <= n; i++) {
        const int p = factor[i];
        if (p == i) {
            f[i] = modexp(p, k) - 1;
        } else if (factor[i / p] == p) {
            f[i] = modmul(f[p] + 1, f[i / p]);
        } else {
            f[i] = modmul(f[p], f[i / p]);
        }
    }
    int result = 1;
    for (int i = 1; i <= n; i++) {
        result = modmul(result, f[i]);
    }
    cout << result << '\n';
}
int main() {
    // sieve
    for (int i = 2; i < 1000; i++) {
        if (factor[i] == 0) {
            for (int j = i*i; j <= 1000000; j += i) {
                if (factor[j] == 0) factor[j] = i;
            }
        }
    }
    for (int i = 2; i <= 1000000; i++) {
        if (factor[i] == 0) factor[i] = i;
    }
    int T; cin >> T;
    while (T--) do_testcase();
}
