// 2023-10-28
// The formula for the area of the ellipse is explained below. The main thing
// that's tricky about this problem is that using doubles can lead to
// catastrophic cancellation, so instead we do some operations using `__int128`
// and some using a user-defined `int256` type.
//
// In general the discriminant of a conic section doesn't indicate whether or
// not the conic is degenerate. However, it turns out that an ellipse can be
// degenerate only when all its points coincide---in which case we get a
// discriminant of 0 anyway (so this program will classify it as a parabola, and
// say "IMPOSSIBLE"). The problem statement doesn't say whether or not this is
// the right answer, and it seems that it doesn't matter anyway (i.e. this case
// doesn't arise in the input).
#include <algorithm>
#include <array>
#include <cmath>
#include <stdio.h>
#include <vector>
using namespace std;

int perm_sign(int* a) {
    int inv = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < i; j++) {
            if (a[i] < a[j]) inv++;
        }
    }
    return 1-2*(inv%2);
}

struct int256 {
    bool sign;
    unsigned __int128 hi;
    unsigned __int128 lo;
};

int256 add_unsigned(int256 x, int256 y) {
    int256 result = {false, x.hi + y.hi, x.lo + y.lo};
    if (result.lo < x.lo) {
        ++result.hi;
    }
    return result;
}

int256 sub_unsigned(int256 x, int256 y) {
    int256 result = {false, x.hi - y.hi, x.lo - y.lo};
    if (result.lo > x.lo) {
        --result.hi;
    }
    return result;
}

bool cmpg_unsigned(int256 x, int256 y) {
    return (x.hi > y.hi || (x.hi == y.hi && x.lo > y.lo));
}

bool cmpge_unsigned(int256 x, int256 y) {
    return (x.hi > y.hi || (x.hi == y.hi && x.lo >= y.lo));
}

int256 operator+(int256 x, int256 y) {
    if (x.sign) {
        if (y.sign) {
            int256 result = add_unsigned(x, y);
            result.sign = true;
            return result;
        } else {
            if (cmpg_unsigned(x, y)) {
                int256 result = sub_unsigned(x, y);
                result.sign = true;
                return result;
            } else {
                return sub_unsigned(y, x);
            }
        }
    } else {
        if (y.sign) {
            if (cmpge_unsigned(x, y)) {
                return sub_unsigned(x, y);
            } else {
                int256 result = sub_unsigned(y, x);
                result.sign = true;
                return result;
            }
        } else {
            return add_unsigned(x, y);
        }
    }
}

int256 operator-(int256 x, int256 y) {
    if (y.sign) {
        y.sign = false;
    } else {
        if (y.hi != 0 || y.lo != 0) y.sign = true;
    }
    return x + y;
}

int256 mul256_unsigned(unsigned __int128 x, unsigned __int128 y) {
    const unsigned __int128 xh = x >> 64;
    const unsigned __int128 xl = x - (xh << 64);
    const unsigned __int128 yh = y >> 64;
    const unsigned __int128 yl = y - (yh << 64);
    int256 result = {false, xh * yh, xl * yl};
    const unsigned __int128 a = xh * yl;
    const unsigned __int128 b = xl * yh;
    result.hi += a >> 64;
    result.hi += b >> 64;
    result.lo += a << 64;
    if (result.lo < (a << 64)) ++result.hi;
    result.lo += b << 64;
    if (result.lo < (b << 64)) ++result.hi;
    return result;
}

int256 mul256(__int128 x, __int128 y) {
    const bool sign = (x < 0 && y > 0) || (x > 0 && y < 0);
    auto result = mul256_unsigned((x < 0 ? -x : x), (y < 0 ? -y : y));
    result.sign = sign;
    return result;
}

int256 mul256_unsigned(int256 x, unsigned __int128 y) {
    const int256 lo = mul256_unsigned(x.lo, y);
    int256 hi = mul256_unsigned(x.hi, y);
    hi.hi = hi.lo;
    hi.lo = 0;
    return add_unsigned(hi, lo);
}

int256 operator*(int256 x, __int128 y) {
    const bool sign = (x.sign && y > 0) ||
                      (!x.sign && y < 0 && (x.hi != 0 || x.lo != 0));
    if (y < 0) y = -y;
    auto result = mul256_unsigned(x, y);
    result.sign = sign;
    return result;
}

double to_double(int256 x) {
    const double result = (pow(2, 128) * (double)x.hi) + (double)x.lo;
    return x.sign ? -result : result;
}

int main() {
    // Generate all permutations, with sign. We do this outside the loop because
    // repetitively calculating the sign of each permutation for every test case
    // causes TLE.
    vector<array<int, 7>> perms;
    array<int, 7> perm = {0, 1, 2, 3, 4, 5};
    do {
        perm[6] = perm_sign(perm.begin());
        perms.push_back(perm);
    } while (next_permutation(perm.begin(), perm.begin() + 6));
    for (;;) {
        /* Use the determinant
           | x_1^2 x_1*y_1 y_1^2 x_1 y_1 1 |
           | x_2^2 x_2*y_2 y_2^2 x_2 y_2 1 |
           | x_3^2 x_3*y_3 y_3^2 x_3 y_3 1 |
           | x_4^2 x_4*y_4 y_4^2 x_4 y_4 1 |
           | x_5^2 x_5*y_5 y_5^2 x_5 y_5 1 |
           |  x^2    x*y    y^2   x   y  1 |
           to solve for the coefficients in
           Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0.
          
           We have
           * |A|, |B|, |C| <= 1.2e20
           * |D|, |E| <= 1.2e23
           * |F| <= 1.2e26
           so we need 128-bit integers. Later on we will need 256 bits.
        */
        long long q[5][6];
        for (int i = 0; i < 5; i++) {
            int x, y; scanf("%d %d", &x, &y);
            q[i][0] = x*x;
            q[i][1] = x*y;
            q[i][2] = y*y;
            q[i][3] = x;
            q[i][4] = y;
            q[i][5] = 1;
        }
        if (feof(stdin)) return 0;
        __int128 coeff[6] = {0};
        for (const auto& perm : perms) {
            // This is the inner loop (it runs 720 times for each test case) so
            // we optimize it by using only a single 128-bit multiplication on
            // each iteration. The two halves of the product each fit into a
            // 64-bit integer. If this optimization isn't done, the program is
            // right on the edge of AC (it gets TLE sometimes).
            const long long prod1 =
              q[0][perm[0]] * q[1][perm[1]] * q[2][perm[2]];
            const long long prod2 =
              q[3][perm[3]] * q[4][perm[4]] * perm[6];
            // Note that GCC is smart enough to optimize this operation into a
            // single IMUL instruction (which takes two 64-bit operands and
            // gives the 128-bit result in RDX:RAX) because it knows that
            // `prod1` still fits in 64 bits even after the cast.
            const __int128 prod = static_cast<__int128>(prod1) * prod2;
            coeff[perm[5]] += prod;
        }
        __int128 A = coeff[0], B = coeff[1], C = coeff[2],
                 D = coeff[3], E = coeff[4], F = coeff[5];
        const int256 disc = mul256(4 * A, C) - mul256(B, B);
        if (disc.sign || (disc.hi == 0 && disc.lo == 0)) {
            // 4AC - B^2 is zero for a parabola, negative for a hyperbola
            puts("IMPOSSIBLE"); continue;
        }
        const int256 t1 = mul256(E, E) * A;
        const int256 t2 = mul256(D, D) * C;
        const int256 t3 = mul256(D, E) * B;
        const int256 t4 = disc * F;
        const double num = to_double(t1 + t2 - t3 - t4);
        const double den = pow(to_double(disc), 1.5);
        printf("%.10f\n", fabs(2*M_PI*num/den));
    }
}
