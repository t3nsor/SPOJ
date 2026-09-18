// 2026-09-18
// The only tricky thing about this problem is that the coefficients are given
// in the reverse of the order you expect.
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;
using LL = long long;
using Matrix = int[15][15];
void mul(int N, Matrix A, Matrix B, int mod, Matrix out) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            LL x = 0;
            for (int k = 0; k < N; k++) x += LL(A[i][k])*B[k][j];
            out[i][j] = x % mod;
        }
    }
}
void pmod(int N, Matrix A, LL e, int mod, Matrix out) {
    if (e == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                out[i][j] = (i == j) % mod;
            }
        }
        return;
    }
    Matrix B;
    pmod(N, A, e / 2, mod, B);
    if (e & 1) {
        Matrix C;
        mul(N, B, B, mod, C);
        mul(N, A, C, mod, out);
    } else {
        mul(N, B, B, mod, out);
    }
}
/// Calculate A^0 + ... + A^{n-1}.  Write the result into `sum` and A^n to
/// `pwr`.
void geosum(int N, Matrix A, LL n, int mod, Matrix sum, Matrix pwr) {
    if (n == 0) {
        memset(sum, 0, sizeof(Matrix));
        pmod(N, A, 0, mod, pwr);  // identity matrix
        return;
    }
    Matrix B, C;
    geosum(N, A, n / 2, mod, B, C);
    if (n & 1) {
        Matrix D;
        mul(N, C, C, mod, D);
        mul(N, A, D, mod, pwr);
        for (int i = 0; i < N; i++) {
            C[i][i] = (C[i][i] + 1) % mod;
        }
        mul(N, C, B, mod, sum);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                sum[i][j] = (sum[i][j] + D[i][j]) % mod;
            }
        }
    } else {
        mul(N, C, C, mod, pwr);
        for (int i = 0; i < N; i++) {
            C[i][i] = (C[i][i] + 1) % mod;
        }
        mul(N, C, B, mod, sum);
    }
}
void do_testcase() {
    int N; cin >> N;
    Matrix A = {0};
    for (int i = 1; i < N; i++) A[i - 1][i] = 1;
    int v[15] = {0};
    for (int i = 0; i < N; i++) cin >> v[i];
    for (int i = N - 1; i >= 0; i--) cin >> A[N - 1][i];
    LL m, n; cin >> m >> n; --m;
    int mod; cin >> mod;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) A[i][j] %= mod;
    }
    // need to calculate A^m + ... + A^{n-1}
    Matrix B;
    pmod(N, A, m, mod, B);
    Matrix C, D;
    geosum(N, A, n - m, mod, C, D);  // the out value D is unused here
    mul(N, B, C, mod, D);
    LL result = 0;
    for (int i = 0; i < N; i++) {
        result = (result + LL(D[0][i]) * v[i]) % mod;
    }
    cout << result << '\n';
}
int main() {
    int C; cin >> C; while (C--) do_testcase();
}
