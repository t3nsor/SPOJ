// 2026-09-30
// Since the input matrices are specified to be generated randomly, we should
// assume that for any large matrix, for all of the columns except for the last
// few, it is highly likely that a pivot can be found that is invertible in
// Z_m (I have renamed `P` to `m` because it's not always prime).  That being
// the case, the approach taken here is to first try to find and use such a
// pivot, which is the fast path.
//
// If such a pivot isn't available, the slow path is taken: when processing the
// ith column, we process rows i, ..., N (assuming for sake of illustration
// that the rows are indexed from 1) starting from rows N and N - 1 and then
// proceeding backward.  When processing a pair of adjacent rows `j` and `j +
// 1` where the elements A_{j,i} and A_{j+1,i} are respectively `a` and `b`,
// the objective is, using elementary row operations involving only these two
// rows, to replace `a` and `b` with, respectively, gcd(a, b) and 0.  The effect
// of this sequence of operations will be to replace each row with some linear
// combination of the two rows, the coefficients of which can be found using the
// extended Euclidean algorithm.  After processing rows `i` and `i+1` we can
// move on to the next column.

#include <stdio.h>
#include <utility>
using namespace std;
using LL = long long;
int modexp(int b, int e, int m) {
    if (e == 0) return 1;
    LL r = modexp(b, e >> 1, m);
    r = (r * r) % m;
    if (e & 1) r = (b * r) % m;
    return r;
}

int gcd(int x, int y) { return x == 0 ? y : gcd(y % x, x); }

/// Store into c, d, e, f values such that
/// a | x
/// b | y
/// would be transformed by elementary row operations into
/// gcd(a,b) | cx + dy
/// 0        | ex + fy
/// and return an overall sign (-1 to the power of the # of row exchanges).
int extended_gcd(int a, int b, LL& c, LL& d, LL& e, LL& f) {
    int s = 1; c = f = 1; d = e = 0;
    while (b) {
        const int q = a/b;
        s *= -1;
        a = exchange(b, a - q*b);
        c = exchange(e, c - q*e);
        d = exchange(f, d - q*f);
    }
    return s;
}

int N, m;
int A[200][200];

int totient(int x) {
    int result = 1;
    while (x > 1) {
        int p = x;
        for (int y = 2; y*y <= x; y++) {
            if (x % y == 0) { p = y; break; }
        }
        result *= p - 1;
        x /= p;
        while (x % p == 0) {
            x /= p;
            result *= p;
        }
    }
    return result;
}

int do_testcase() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &A[i][j]);
            A[i][j] = ((A[i][j] % m) + m) % m;
        }
    }
    if (m == 1) return 0;
    const int phi = totient(m);
    LL result = 1;
    for (int col = 0; col < N; ++col) {
        int pivot = -1;
        for (int row = col; row < N; row++) {
            if (gcd(m, A[row][col]) == 1) {
                pivot = row;
                break;
            }
        }
        if (pivot >= 0) {
            // fast path
            if (pivot > col) {
                result = (m - result) % m;
                for (int col2 = col; col2 < N; col2++) {
                    swap(A[col][col2], A[pivot][col2]);
                }
            }
            const LL inv = modexp(A[col][col], phi - 1, m);
            result = (result*A[col][col]) % m;
            for (int col2 = col; col2 < N; col2++) {
                A[col][col2] = (A[col][col2]*inv) % m;
            }
            for (int row = col + 1; row < N; row++) {
                const LL factor = (m - A[row][col]) % m;
                for (int col2 = col; col2 < N; col2++) {
                    A[row][col2] = (A[row][col2] + factor*A[col][col2]) % m;
                }
            }
            continue;
        }
        // slow path
        for (int row = N - 2; row >= col; row--) {
            const int a = A[row][col];
            const int b = A[row + 1][col];
            if (b == 0) continue;
            LL c, d, e, f;
            int s = extended_gcd(a, b, c, d, e, f);
            c = ((c % m) + m) % m;
            d = ((d % m) + m) % m;
            e = ((e % m) + m) % m;
            f = ((f % m) + m) % m;
            if (s < 0) result = (m - result) % m;
            for (int col2 = col; col2 < N; col2++) {
                const int x = A[row][col2];
                const int y = A[row + 1][col2];
                A[row][col2] = (c*x + d*y) % m;
                A[row + 1][col2] = (e*x + f*y) % m;
            }
        }
        result = (result * A[col][col]) % m;
        if (result == 0) break;
    }
    return result;
}

int main() {
    while (2 == scanf("%d %d", &N, &m)) {
        printf("%d\n", do_testcase());
    }
}
