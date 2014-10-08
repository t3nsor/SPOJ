// 2014-05-03
#include <cstdio>
#include <cstring>
using namespace std;
const int PRIME = 1000000007;
typedef long long matrix[2][2];

void mul(matrix m1, matrix m2, matrix& res) {
    memset(res, 0, sizeof res);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                res[i][j] += m1[i][k] * m2[k][j];
            }
            res[i][j] %= PRIME;
        }
    }
}

void pwr(matrix m, long long p, matrix& res) {
    matrix t; memcpy(t, m, sizeof t);
    res[0][1] = res[1][0] = 0;
    res[0][0] = res[1][1] = 1;
    while (p) {
        if (p & 1) {
            matrix tmp;
            mul(t, res, tmp);
            memcpy(res, tmp, sizeof res);
        }
        p >>= 1;
        matrix tmp; memcpy(tmp, t, sizeof tmp);
        mul(tmp, tmp, t);
    }
}

void extended_euclid(long long a, long long b, long long &x, long long &y) {
    long long xx = y = 0;
    long long yy = x = 1;
    while (b) {
        long long q = a/b;
        long long t = b; b = a%b; a = t;
        t = xx; xx = x-q*xx; x = t;
        t = yy; yy = y-q*yy; y = t;
    }
}

long long mod_inverse(long long a, long long n) {
    long long x, y;
    extended_euclid(a, n, x, y);
    return ((x%n)+n)%n;
}

void inv(matrix m, matrix& res) {
    long long det = m[0][0] * m[1][1] - m[0][1] * m[1][0];
    det = (det%PRIME + PRIME)%PRIME;
    long long idet = mod_inverse(det, PRIME);
    res[0][0] = (m[1][1]*idet)%PRIME;
    res[0][1] = ((PRIME - m[0][1])*idet)%PRIME;
    res[1][0] = ((PRIME - m[1][0])*idet)%PRIME;
    res[1][1] = (m[0][0]*idet)%PRIME;
}

matrix id = {{1, 0}, {0, 1}};
matrix fib = {{0, 1}, {1, 1}};
matrix fib_pwr[65536];
matrix fib_inv[65536];

int main() {
    memcpy(fib_pwr[0], id, sizeof(matrix));
    for (int i = 1; i < 65536; i++) {
        mul(fib_pwr[i-1], fib, fib_pwr[i]);
        matrix t; memcpy(t, fib_pwr[i], sizeof(matrix));
        t[0][0] = (t[0][0] - 1 + PRIME)%PRIME;
        t[1][1] = (t[1][1] - 1 + PRIME)%PRIME;
        inv(t, fib_inv[i]);
    }
    int T; scanf("%d", &T);
    while (T--) {
        long long c, k, N;
        scanf("%lld %lld %lld", &c, &k, &N);
        matrix num; pwr(fib_pwr[k], N, num);
        num[0][0] = (num[0][0] - 1 + PRIME)%PRIME;
        num[1][1] = (num[1][1] - 1 + PRIME)%PRIME;
        matrix sum; mul(num, fib_inv[k], sum);
        matrix res; mul(sum, fib_pwr[k+c], res);
        printf("%lld\n", res[0][1]);
    }
}
