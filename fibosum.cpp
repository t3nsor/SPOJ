// 2014-04-26
#include <iostream>
#include <cstring>
using namespace std;
const int PRIME = 1000000007;
typedef long long matrix[2][2];
typedef long long vec[2];
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
void pwr(matrix m, int p, matrix& res) {
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
void apply(matrix m, vec v, vec& res) {
    memset(res, 0, sizeof res);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            res[i] += m[i][j] * v[j];
        }
        res[i] %= PRIME;
    }
}
long long getfib(int idx) {
    vec v; v[0] = 0; v[1] = 1;
    matrix fib; fib[0][0] = 0; fib[0][1] = fib[1][0] = fib[1][1] = 1;
    matrix p; pwr(fib, idx, p);
    vec res; apply(p, v, res);
    return res[0];
}
int main() {
    int T; cin >> T;
    while (T--) {
        int N, M;
        cin >> N >> M;
        cout << (getfib(M+2) - getfib(N+1) + PRIME) % PRIME << endl;
    }
}
