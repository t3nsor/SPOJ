// 2025-11-16
// There are probably a lot of different ways to solve this problem.  This
// solution is probably not the simplest.
//
// Each "0" or "1" instruction is an operator that acts on the state space,
// which consists of a location and a direction.  By represeting the state as a
// complex vector where the first entry represents location (x + yi representing
// the coordinates (x, y)) and the second direction (which is always 1, i, -1,
// or -i) we can represent the action of each "0" or "1" instruction as a 2x2
// matrix and concatenation as matrix multiplication.  The problem then reduces
// to using the structure of the `s` strings to efficiently construct the
// required products.
#include <array>
#include <iostream>
#include <limits.h>
using namespace std;
using ll = long long;
struct cmplx {
    ll re;
    ll im;
};
cmplx zero = {0, 0};
cmplx one = {1, 0};
cmplx plusi = {0, 1};
cmplx minusi = {0, -1};
cmplx operator+(cmplx x1, cmplx x2) {
    return {x1.re + x2.re, x1.im + x2.im};
}
cmplx operator*(cmplx x1, cmplx x2) {
    return {x1.re*x2.re - x1.im*x2.im, x1.re*x2.im + x2.re*x1.im};
}
using Mat = array<array<cmplx, 2>, 2>;
Mat operator*(Mat m1, Mat m2) {
    Mat result;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result[i][j] = m1[i][0]*m2[0][j] + m1[i][1]*m2[1][j];
        }
    }
    return result;
}
Mat M0 = {{{one, one}, {zero, minusi}}};
Mat M1 = {{{one, one}, {zero, plusi}}};
Mat s[2][31];
Mat getm(int i, int sl, int sr, int r) {
    if (r >= sr) return s[(sl>>(i+1))&1][i];
    int mid = sl + (sr - sl)/2;
    Mat result = getm(i - 1, sl, mid - 1, r);
    if (r >= mid) {
        result = ((sl>>(i+1))&1 ? M0 : M1) * result;
    }
    if (r > mid) {
        result = getm(i - 1, mid + 1, sr, r) * result;
    }
    return result;
}
int main() {
    s[0][0] = M1;
    s[1][0] = M0;
    for (int i = 1; i <= 30; i++) {
        s[0][i] = s[1][i - 1] * M1 * s[0][i - 1];
        s[1][i] = s[1][i - 1] * M0 * s[0][i - 1];
    }
    for (;;) {
        int n; cin >> n;
        if (n < 0) break;
        if (n == 0) { puts("(0,0)"); continue; }
        Mat m = getm(30, 0, INT_MAX - 1, n - 1);
        printf("(%lld,%lld)\n", m[0][1].re, m[0][1].im);
        fflush(stdout);
    }
}
