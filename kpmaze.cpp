// 2024-09-24
// To solve this problem, we apply Kirchhoff's matrix-tree theorem. To get an
// exact result, we should perform row reduction modulo some prime.  An upper
// bound for the number of possible mazes is 40 choose 16 = 62852101650, so we
// can use (40 choose 16) + 1 (which turns out to be prime) as the modulus, but
// some intermediate results need to be computed using 128-bit arithmetic. In
// actual fact the maximum possible result fits in 32 bits, so the intermediate
// calculations could be done using 64-bit arithmetic only, but this is not
// a priori obvious (at least to me, because I'm bad at math), i.e., all the
// `unsigned __int128`s below could be replaced by `unsigned long long` if an
// appropriate 32-bit prime is chosen. When
// this problem was set in 2006, the judge's C++ compiler probably did not
// support emulated 128 bit arithmetic in C++, so if you weren't able to somehow
// figure out that 64-bit arithmetic is sufficient, you would have to compute
// the determinant modulo two different primes that fit in 32 bits and use the
// Chinese remainder theorem to reconstruct the actual value (annoying, not
// shown below).
#include <iostream>
#include <string.h>
#include <utility>
#include <vector>
using namespace std;
constexpr unsigned long long MOD = 62852101651;
unsigned long long pmod(unsigned long long b, unsigned long long e) {
    if (e == 0) return 1;
    unsigned __int128 result = pmod(b, e / 2);
    result = (result * result) % MOD;
    if (e & 1) {
        result = (result * b) % MOD;
    }
    return result;
}
unsigned long long modinv(unsigned long long x) {
    return pmod(x, MOD - 2);
}
unsigned long long matrix[25][25];
unsigned long long det(int N) {
    unsigned __int128 result = 1;
    for (int i = 0; i < N; i++) {
        int j = i;
        while (j < N && matrix[i][j] == 0) j++;
        if (j == N) return 0;
        swap(matrix[i], matrix[j]);
        result = (result * matrix[i][i]) % MOD;
        const unsigned __int128 inv = modinv(matrix[i][i]);
        for (int j = i; j < N; j++) {
            matrix[i][j] = (matrix[i][j] * inv) % MOD;
        }
        for (int k = 0; k < N; k++) {
            if (k == i) continue;
            const unsigned __int128 factor = (MOD - matrix[k][i]) % MOD;
            for (int j = i; j < N; j++) {
                matrix[k][j] = (matrix[k][j] + factor * matrix[i][j]) % MOD;
            }
        }
    }
    return result;
}
int main() {
    int R, C; cin >> C >> R;
    int K; cin >> K;
    bool rwall[5][5] = {0}, bwall[5][5] = {0};
    for (int i = 0; i < R; i++) {
        rwall[i][C - 1] = true;
    }
    for (int j = 0; j < C; j++) {
        bwall[R - 1][j] = true;
    }
    while (K--) {
        int R1, C1, R2, C2; cin >> R1 >> C1 >> R2 >> C2; --R1; --C1; --R2; --C2;
        if (R1 == R2) {
            if (C1 > C2) swap(C1, C2);
            rwall[R1][C1] = true;
        } else {
            if (R1 > R2) swap(R1, R2);
            bwall[R1][C1] = true;
        }
    }
    memset(matrix, 0, sizeof(matrix));
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            const int u = i * C + j;
            if (!rwall[i][j]) {
                const int v = i * C + j + 1;
                matrix[u][u]++;
                matrix[v][v]++;
                matrix[u][v] = (matrix[u][v] + MOD - 1) % MOD;
                matrix[v][u] = (matrix[v][u] + MOD - 1) % MOD;
            }
            if (!bwall[i][j]) {
                const int v = (i + 1) * C + j;
                matrix[u][u]++;
                matrix[v][v]++;
                matrix[u][v] = (matrix[u][v] + MOD - 1) % MOD;
                matrix[v][u] = (matrix[v][u] + MOD - 1) % MOD;
            }
        }
    }
    printf("%llu\n", det(R * C - 1));
}
