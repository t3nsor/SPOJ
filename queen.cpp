// 2014-04-29
#include <cstdio>
#include <queue>
#include <cstring>
using namespace std;
const int dx[8] = {1, 1, 1, 0, 0, -1, -1, -1};
const int dy[8] = {1, 0, -1, 1, -1, 1, 0, -1};
char grid[1003][1003];
char vis[1024][1024];
struct node {
    int r;
    int c;
    int dir;
    int dist;
};
int main() {
    int t; scanf("%d", &t);
    while (t--) {
        int R, C; scanf("%d %d\n", &R, &C);
        int sr, sc, fr, fc;
        for (int i = 1; i <= R; i++) {
            gets(grid[i]+1);
            char* s = strchr(grid[i]+1, 'S');
            if (s) {
                sr = i; sc = s - grid[i];
            }
            char* f = strchr(grid[i]+1, 'F');
            if (f) {
                fr = i; fc = f - grid[i];
            }
        }
        R += 2; C += 2;
        for (int i = 0; i < R; i++) {
            grid[i][0] = grid[i][C-1] = 'X';
        }
        for (int i = 0; i < C; i++) {
            grid[0][i] = grid[R-1][i] = 'X';
        }
        memset(vis, 0, R*1024);
        queue<node> D;
        for (int i = 0; i < 8; i++) {
            D.push((node){sr, sc, i, 0});
        }
        vis[sr][sc] = 255;
        int res = -1;
        while (!D.empty()) {
            node N = D.front(); D.pop();
            if (N.r == fr && N.c == fc) {
                res = N.dist; break;
            }
            node N2; N2.dist = N.dist+1;
            {
                N2.dir = 0; N2.r = N.r + dx[0]; N2.c = N.c + dy[0];
                while (grid[N2.r][N2.c] != 'X' && !(vis[N2.r][N2.c] & (1<<0))) {
                    if (!vis[N2.r][N2.c]) {
                        D.push(N2);
                    }
                    vis[N2.r][N2.c] |= 1<<0;
                    N2.r += dx[0]; N2.c += dy[0];
                }
            }
            {
                N2.dir = 1; N2.r = N.r + dx[1]; N2.c = N.c + dy[1];
                while (grid[N2.r][N2.c] != 'X' && !(vis[N2.r][N2.c] & (1<<1))) {
                    if (!vis[N2.r][N2.c]) {
                        D.push(N2);
                    }
                    vis[N2.r][N2.c] |= 1<<1;
                    N2.r += dx[1]; N2.c += dy[1];
                }
            }
            {
                N2.dir = 2; N2.r = N.r + dx[2]; N2.c = N.c + dy[2];
                while (grid[N2.r][N2.c] != 'X' && !(vis[N2.r][N2.c] & (1<<2))) {
                    if (!vis[N2.r][N2.c]) {
                        D.push(N2);
                    }
                    vis[N2.r][N2.c] |= 1<<2;
                    N2.r += dx[2]; N2.c += dy[2];
                }
            }
            {
                N2.dir = 3; N2.r = N.r + dx[3]; N2.c = N.c + dy[3];
                while (grid[N2.r][N2.c] != 'X' && !(vis[N2.r][N2.c] & (1<<3))) {
                    if (!vis[N2.r][N2.c]) {
                        D.push(N2);
                    }
                    vis[N2.r][N2.c] |= 1<<3;
                    N2.r += dx[3]; N2.c += dy[3];
                }
            }
            {
                N2.dir = 4; N2.r = N.r + dx[4]; N2.c = N.c + dy[4];
                while (grid[N2.r][N2.c] != 'X' && !(vis[N2.r][N2.c] & (1<<4))) {
                    if (!vis[N2.r][N2.c]) {
                        D.push(N2);
                    }
                    vis[N2.r][N2.c] |= 1<<4;
                    N2.r += dx[4]; N2.c += dy[4];
                }
            }
            {
                N2.dir = 5; N2.r = N.r + dx[5]; N2.c = N.c + dy[5];
                while (grid[N2.r][N2.c] != 'X' && !(vis[N2.r][N2.c] & (1<<5))) {
                    if (!vis[N2.r][N2.c]) {
                        D.push(N2);
                    }
                    vis[N2.r][N2.c] |= 1<<5;
                    N2.r += dx[5]; N2.c += dy[5];
                }
            }
            {
                N2.dir = 6; N2.r = N.r + dx[6]; N2.c = N.c + dy[6];
                while (grid[N2.r][N2.c] != 'X' && !(vis[N2.r][N2.c] & (1<<6))) {
                    if (!vis[N2.r][N2.c]) {
                        D.push(N2);
                    }
                    vis[N2.r][N2.c] |= 1<<6;
                    N2.r += dx[6]; N2.c += dy[6];
                }
            }
            {
                N2.dir = 7; N2.r = N.r + dx[7]; N2.c = N.c + dy[7];
                while (grid[N2.r][N2.c] != 'X' && !(vis[N2.r][N2.c] & (1<<7))) {
                    if (!vis[N2.r][N2.c]) {
                        D.push(N2);
                    }
                    vis[N2.r][N2.c] |= 1<<7;
                    N2.r += dx[7]; N2.c += dy[7];
                }
            }
        }
        printf("%d\n", res);
    }
}
