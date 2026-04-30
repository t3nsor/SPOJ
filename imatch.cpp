// 2026-04-29
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
double d[200][200];
double d2[200][200];
void do_testcase() {
    int N; scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int x; scanf("%d", &x);
            if (x == 0) d[i][j] = 1e6; else d[i][j] = -log(x/100.0);
        }
    }
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
            }
        }
    }
    int M; scanf("%d", &M);
    vector<int> accts;
    for (int i = 0; i < M; i++) {
        int x; scanf("%d", &x); accts.push_back(x - 1);
    }
    sort(accts.begin(), accts.end());
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            d2[i][j] = min(exp(d[accts[i]][accts[j]]), 1e9);
        }
    }
    for (int k = 0; k < M; k++) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < M; j++) {
                d2[i][j] = min(d2[i][j], d2[i][k] + d2[k][j]);
            }
        }
    }
    int S; scanf("%d", &S);
    printf("%.7f\n", S*d2[0][1]);
}
int main() {
    int T; scanf("%d", &T); while (T--) do_testcase();
}
