// 2014-10-02
#include <cstdio>
#include <queue>
#include <cstring>
#include <utility>
using namespace std;
const int dx[4] = {1, 0, -1, 0};
const int dy[4] = {0, 1, 0, -1};
int main() {
    int grid[100][100];
    int dist[100][100];
    int K; scanf("%d", &K);
    while (K--) {
        int M, N; scanf("%d %d", &M, &N);
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                scanf("%d", grid[i] + j);
            }
        }
        int a, b, T; scanf("%d %d %d", &a, &b, &T);
        a--; b--;
        memset(dist, 0x3f, sizeof dist);
        priority_queue<pair<int, pair<int, int> > > Q;
        Q.push(make_pair(-grid[0][0], make_pair(0, 0)));
        while (!Q.empty()) {
            pair<int, pair<int, int> > P = Q.top();
            Q.pop();
            if (dist[P.second.first][P.second.second] < 0x3f3f3f3f) {
                continue;
            }
            dist[P.second.first][P.second.second] = -P.first;
            if (P.second == make_pair(a, b)) {
                break;
            }
            for (int i = 0; i < 4; i++) {
                int r = P.second.first + dx[i];
                int c = P.second.second + dy[i];
                if (r < 0 || c < 0 || r >= M || c >= N) continue;
                Q.push(make_pair(P.first-grid[r][c], make_pair(r, c)));
            }
        }
        if (dist[a][b] <= T) {
            printf("YES\n%d\n", T - dist[a][b]);
        } else {
            puts("NO");
        }
    }
    return 0;
}
