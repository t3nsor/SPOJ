// 2023-04-02
// This is basically a pure DP problem; see below for more details.
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

template <int D> struct ndvector : vector<ndvector<D - 1>> {
    template <typename Head, typename... Tail>
    ndvector(Head size, Tail... tail)
      : vector<ndvector<D - 1>>(size, ndvector<D - 1>(tail...)) {}
};

template <> struct ndvector<1> : vector<int> {
    using vector<int>::vector;
};

struct Pair {
    int b;
    int c;
};

struct Triple {
    int b;
    int c;
    int d;
};

void do_testcase() {
    int N, M, U, V; cin >> N >> M >> U >> V;
    V -= U;  // V is the number of rules to convert 1 number to 3 numbers
    vector<int> X(N);
    for (int i = 0; i < N; i++) {
        cin >> X[i]; --X[i];
    }
    vector<int> Y(M);
    for (int i = 0; i < M; i++) {
        cin >> Y[i]; --Y[i];
    }
    vector<vector<Pair>> rules2(30);
    for (int i = 0; i < U; i++) {
        int a, b, c; cin >> a >> b >> c; --a; --b; --c;
        rules2[a].push_back({b, c});
    }
    vector<vector<Triple>> rules3(30);
    for (int i = 0; i < V; i++) {
        int a, b, c, d; cin >> a >> b >> c >> d; --a; --b; --c; --d;
        rules3[a].push_back({b, c, d});
    }
    // dp1[i][j][k] is the minimum moves required to transform a sequence
    // consisting of a single integer `i` into the range Y[j] ... Y[k-1]
    //
    // dp2[i1][i2][j][k] is similar but starting with a sequence consisting of
    // two integers, `i1` and `i2`
    //
    // dp[i][j][k][l] is the minimum moves required to transform
    // X[i] ... X[j-1] to Y[k] ... Y[k-1]
    ndvector<3> dp1(30, M + 1, M + 1, 100);
    ndvector<4> dp2(30, 30, M + 1, M + 1, 100);
    for (int len = 1; len <= M; len++) {
        for (int k = len; k <= M; k++) {
            int j = k - len;
            for (int i1 = 0; i1 < 30; i1++) {
                for (int i2 = 0; i2 < 30; i2++) {
                    for (int m = j + 1; m < k; m++) {
                        dp2[i1][i2][j][k] = min(dp2[i1][i2][j][k],
                                                dp1[i1][j][m] + dp1[i2][m][k]);
                    }
                }
            }
            for (int i = 0; i < 30; i++) {
                if (len == 1) {
                    if (i == Y[j]) dp1[i][j][k] = 0;
                    continue;
                }
                // apply rules
                for (const auto& p : rules2[i]) {
                    dp1[i][j][k] = min(dp1[i][j][k], 1 + dp2[p.b][p.c][j][k]);
                }
                for (const auto& t : rules3[i]) {
                    for (int m = j + 1; m < k; m++) {
                        dp1[i][j][k] =
                          min(dp1[i][j][k],
                              1 + min(dp1[t.b][j][m] + dp2[t.c][t.d][m][k],
                                      dp2[t.b][t.c][j][m] + dp1[t.d][m][k]));
                    }
                }
            }
        }
    }
    ndvector<4> dp(N + 1, N + 1, M + 1, M + 1, 100);
    for (int xlen = 1; xlen <= N; xlen++) {
        for (int j = xlen; j <= N; j++) {
            int i = j - xlen;
            for (int ylen = xlen; ylen <= M; ylen++) {
                for (int l = ylen; l <= M; l++) {
                    int k = l - ylen;
                    if (xlen == 1) {
                        dp[i][j][k][l] = dp1[X[i]][k][l];
                        continue;
                    } else if (xlen == 2) {
                        dp[i][j][k][l] = dp2[X[i]][X[i+1]][k][l];
                        continue;
                    }
                    for (int m1 = i + 1; m1 < j; m1++) {
                        for (int m2 = k + 1; m2 < l; m2++) {
                            dp[i][j][k][l] = min(dp[i][j][k][l],
                                                 dp[i][m1][k][m2] +
                                                 dp[m1][j][m2][l]);
                        }
                    }
                }
            }
        }
    }
    int result = dp[0][N][0][M];
    cout << (result < 50 ? result : -1) << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
