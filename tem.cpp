// 2024-08-31
#include <algorithm>
#include <stdio.h>
#include <utility>
using namespace std;
int input[50][50][50];
int matrix[50][50][50];
int psum[50][51][51];
void do_testcase() {
    int X, Y, Z; scanf("%d %d %d", &X, &Y, &Z);
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            for (int k = 0; k < Z; k++) {
                scanf("%d", &input[i][j][k]);
            }
        }
    }
    // transpose the biggest dimension to the end
    int xpose = 2;
    if (X > Z && X >= Y) {
        for (int i = 0; i < X; i++) {
            for (int j = 0; j < Y; j++) {
                for (int k = 0; k < Z; k++) {
                    matrix[k][j][i] = input[i][j][k];
                }
            }
        }
        swap(X, Z);
        xpose = 0;
    } else if (Y > Z && Y >= X) {
        for (int i = 0; i < X; i++) {
            for (int j = 0; j < Y; j++) {
                for (int k = 0; k < Z; k++) {
                    matrix[i][k][j] = input[i][j][k];
                }
            }
        }
        swap(Y, Z);
        xpose = 1;
    } else {
        for (int i = 0; i < X; i++) {
            for (int j = 0; j < Y; j++) {
                for (int k = 0; k < Z; k++) {
                    matrix[i][j][k] = input[i][j][k];
                }
            }
        }
    }
    // compute prefix sums
    for (int i = 0; i <= X; i++) {
        for (int j = 0; j <= Y; j++) {
            for (int k = 0; k < Z; k++) {
                if (i == 0 || j == 0) {
                    psum[i][j][k] = 0;
                } else {
                    psum[i][j][k] = matrix[i - 1][j - 1][k]
                                    + psum[i - 1][j][k]
                                    + psum[i][j - 1][k]
                                    - psum[i - 1][j - 1][k];
                }
            }
        }
    }
    int bv = -2e9;
    int bx1, bx2, by1, by2, bz1, bz2;
    for (int i1 = 0; i1 < X; i1++) {
        for (int i2 = i1 + 1; i2 <= X; i2++) {
            for (int j1 = 0; j1 < Y; j1++) {
                for (int j2 = j1 + 1; j2 <= Y; j2++) {
                    const auto a = [=](int k) {
                        return psum[i2][j2][k]
                             - psum[i1][j2][k]
                             - psum[i2][j1][k]
                             + psum[i1][j1][k];
                    };
                    int v = a(0);
                    int k1 = 0;
                    if (v > bv) {
                        bv = v;
                        bx1 = i1;
                        bx2 = i2;
                        by1 = j1;
                        by2 = j2;
                        bz1 = 0;
                        bz2 = 1;
                    }
                    for (int k2 = 2; k2 <= Z; k2++) {
                        const int ak = a(k2 - 1);
                        if (ak > v + ak) {
                            v = ak;
                            k1 = k2 - 1;
                        } else {
                            v += ak;
                        }
                        if (v > bv) {
                            bv = v;
                            bx1 = i1;
                            bx2 = i2;
                            by1 = j1;
                            by2 = j2;
                            bz1 = k1;
                            bz2 = k2;
                        }
                    }
                }
            }
        }
    }
    // tranpose back to original coordinates
    if (xpose == 0) {
        swap(bx1, bz1);
        swap(bx2, bz2);
    } else if (xpose == 1) {
        swap(by1, bz1);
        swap(by2, bz2);
    }
    printf("%d %d %d %d %d %d\n",
           bx1 + 1, by1 + 1, bz1 + 1,
           bx2, by2, bz2);
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
