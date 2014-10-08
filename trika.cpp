// 2014-10-01
#include <cstdio>
#include <algorithm>
using namespace std;
int main() {
    int N, M; scanf("%d %d", &N, &M);
    int X, Y; scanf("%d %d", &X, &Y); X--; Y--;
    int grid[20][20];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &grid[i][j]);
        }
    }
    int best[20][20];
    for (int i = X; i < N; i++) {
        for (int j = Y; j < M; j++) {
            if (i == X) {
                if (j == Y) {
                    best[i][j] = grid[i][j];
                } else {
                    best[i][j] = best[i][j-1] - grid[i][j];
                }
            } else if (j == Y) {
                best[i][j] = best[i-1][j] - grid[i][j];
            } else {
                best[i][j] = max(best[i-1][j], best[i][j-1]) - grid[i][j];
            }
        }
    }
    if (best[N-1][M-1] >= 0) {
        printf("Y %d\n", best[N-1][M-1]);
    } else {
        printf("N\n");
    }
}
