// 2023-12-25
#include <iostream>
#include <string.h>
using namespace std;
int memo[11][11][11][31];
constexpr int MOD = 11380;
int calc(int i, int j, int k, int l) {
    int& result = memo[i][j][k][l];
    if (result >= 0) return result;
    result = 0;
    if (l == 0) {
        if (i + j + k == 0) {
            result = 1;
        }
        return result;
    }
    for (int d = 1; d <= l; d++) {
        // Let `d` be the depth of the first piece, which shall be a valid
        // string enclosed in a pair of (some kind of) brackets. The remainder
        // can be empty.
        for (int i1 = 0; i1 <= i; i1++) {
            for (int j1 = 0; j1 <= j; j1++) {
                for (int k1 = 0; k1 <= k; k1++) {
                    if (i1 + j1 + k1 == 0) continue;
                    const int i2 = i - i1;
                    const int j2 = j - j1;
                    const int k2 = k - k1;
                    if (i1 > 0) {
                        // the first piece can be enclosed in {}
                        result += calc(i1 - 1, j1, k1, d - 1) *
                                  calc(i2, j2, k2, l);
                        result %= MOD;
                        // If the first piece uses the full depth, then the
                        // remainder of the string can have any depth.
                        if (d == l) {
                            for (int d2 = 0; d2 < l; d2++) {
                                result += calc(i1 - 1, j1, k1, d - 1) *
                                          calc(i2, j2, k2, d2);
                                result %= MOD;
                            }
                        }
                    }
                    if (j1 > 0 && i1 == 0) {
                        // the first piece can be enclosed in []
                        result += calc(i1, j1 - 1, k1, d - 1) *
                                  calc(i2, j2, k2, l);
                        result %= MOD;
                        if (d == l) {
                            for (int d2 = 0; d2 < l; d2++) {
                                result += calc(i1, j1 - 1, k1, d - 1) *
                                          calc(i2, j2, k2, d2);
                                result %= MOD;
                            }
                        }
                    }
                    if (k1 > 0 && i1 == 0 && j1 == 0) {
                        // the first piece can be enclosed in ()
                        result += calc(i1, j1, k1 - 1, d - 1) *
                                  calc(i2, j2, k2, l);
                        result %= MOD;
                        if (d == l) {
                            for (int d2 = 0; d2 < l; d2++) {
                                result += calc(i1, j1, k1 - 1, d - 1) *
                                          calc(i2, j2, k2, d2);
                                result %= MOD;
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}
int main() {
    memset(memo, -1, sizeof(memo));
    for (int i = 0; i < 10; i++) {
        int L1, L2, L3, D;
        cin >> L1 >> L2 >> L3 >> D;
        cout << calc(L1, L2, L3, D) << '\n';
    }
}
