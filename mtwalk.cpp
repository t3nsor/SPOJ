// 2023-10-30
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utility>
#include <vector>
using namespace std;
const int dx[4]={ 1, 0,-1, 0};
const int dy[4]={ 0,-1, 0, 1};
int dist[100][100];
int main() {
    int elevation[100][100];
    int N; scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", elevation[i] + j);
        }
    }
    int best=1000;
    int l = 0;
    int r = 110;
    while (r > l) {
        const int m = (l + r) / 2;
        bool success = false;
        for (int lo = 0; lo <= 110 - m; lo++) {
            const int hi = lo + m;
            if (elevation[0][0] < lo ||
                elevation[0][0] > hi ||
                elevation[N - 1][N - 1] < lo ||
                elevation[N - 1][N - 1] > hi) {
                continue;
            }
            vector<vector<char>> vis(N, vector<char>(N, 0));
            stack<pair<int,int>> S;
            S.emplace(0, 0);
            while (!S.empty()) {
                auto P = S.top();
                const int r = P.first;
                const int c = P.second;
                S.pop();
                if (vis[r][c]) continue;
                vis[r][c] = true;
                if (r == N - 1 && c == N - 1) break;
                for (int k = 0; k < 4; k++) {
                    const int r2 = r + dx[k];
                    const int c2 = c + dy[k];
                    if (r2 >= 0 && r2 < N && c2 >= 0 && c2 < N &&
                        elevation[r2][c2] >= lo &&
                        elevation[r2][c2] <= hi &&
                        !vis[r2][c2]) {
                        S.emplace(r2, c2);
                    }
                }
            }
            if (vis[N - 1][N - 1]) {
                success = true;
                break;
            }
        }
        if (success) {
            r = m;
        } else {
            l = m + 1;
        }
    }
    printf("%d\n", r);
    return 0;
}
