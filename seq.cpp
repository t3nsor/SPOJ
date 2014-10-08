// 2014-10-02
#include <cstdio>
#include <vector>
using namespace std;
const int MOD = 1000000000;
struct Matrix {
    vector<vector<int> > data;
    Matrix(int N): data(N, vector<int>(N)) {}
    int size() const {
        return data.size();
    }
    vector<int>& operator[](int i) {
        return data[i];
    }
    const vector<int>& operator[](int i) const {
        return data[i];
    }
};
void mul(Matrix& res, const Matrix& A, const Matrix& B) {
    for (int i = 0; i < res.size(); i++) {
        for (int j = 0; j < res.size(); j++) {
            res[i][j] = 0;
            for (int k = 0; k < res.size(); k++) {
                res[i][j] += (A[i][k]*(long long)B[k][j])%MOD;
                if (res[i][j] >= MOD) res[i][j] -= MOD;
            }
        }
    }
}
void pwr(Matrix& res, const Matrix& base, int exponent) {
    for (int i = 0; i < res.size(); i++) {
        for (int j = 0; j < res.size(); j++) {
            res[i][j] = (i==j);
        }
    }
    Matrix power = base;
    int p2 = 1;
    while (p2 <= exponent) {
        if (p2 & exponent) {
            Matrix tmp(res.size());
            mul(tmp, res, power);
            res = tmp;
        }
        Matrix tmp(res.size());
        mul(tmp, power, power);
        power = tmp;
        p2 <<= 1;
    }
}
int main() {
    int C; scanf("%d", &C);
    while (C--) {
        int k; scanf("%d", &k);
        Matrix M(k);
        for (int i = 0; i < k-1; i++) {
            M[i][i+1] = 1;
        }
        int b[10];
        for (int i = 0; i < k; i++) {
            scanf("%d", b+i);
            if (b[i] == MOD) b[i] = 0;
        }
        for (int i = 0; i < k; i++) {
            int x; scanf("%d", &x);
            if (x == MOD) x = 0;
            M[k-1][k-i-1] = x;
        }
        int n; scanf("%d", &n);
        Matrix p(k); pwr(p, M, n-1);
        int res = 0;
        for (int i = 0; i < k; i++) {
            res += (p[0][i]*(long long)b[i])%MOD;
            if (res >= MOD) res -= MOD;
        }
        printf("%d\n", res);
    }
    return 0;
}
