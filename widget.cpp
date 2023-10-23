// 2023-10-23
#include <iostream>
#include <cstring>
#include <vector>
using namespace std;
int N,M; //size of matrix
int M1[300][301];
void row_swap(int r1,int r2) {
    int i;
    for (i=0; i<N+1; i++) {
        swap(M1[r1][i],M1[r2][i]);
    }
}
void row_scale(int r1,int r2,int w1,int w2) {
    int i;
    for (i=0; i<N+1; i++) {
        M1[r1][i]=(w1*M1[r1][i]+w2*M1[r2][i])%7;
    }
}
constexpr int inv[7]={0, 1, 4, 5, 2, 3, 6};
constexpr int sol[7]={7, 8, 9, 3, 4, 5, 6};
void solve() {
    int i,j;
    for (j=0; j<N; j++) {
        //column
        //make M[j][j] equal to 1
        if (!M1[j][j]) {
            //find one that's not
            bool ok=false;
            for (i=0; i<M; i++) {
                if (M1[i][j]) {
                    ok=true;
                    row_swap(i,j);
                    break;
                }
            }
            if (!ok) continue; //skip this one
        }
        row_scale(j,0,inv[M1[j][j]],0);
        for (i=0; i<M; i++) {
            if (i==j) continue;
            row_scale(i,j,1,7-M1[i][j]);
        }
    }
}
int parse_day(const char* s) {
    if (s[0]=='M') return 1;
    if (s[0]=='W') return 3;
    if (s[0]=='F') return 5;
    if (s[1]=='H') return 4;
    if (s[1]=='A') return 6;
    if (s[2]=='N') return 0;
    return 2;
}
void do_testcase(int N, int M) {
    vector<vector<int>> matrix(M, vector<int>(N + 1, 0));
    for (int i = 0; i < M; i++) {
        int n;
        char d1[4], d2[4];
        scanf("%d %s %s", &n, d1, d2);
        matrix[i][N] = (parse_day(d2) - parse_day(d1) + 8) % 7;
        while (n--) {
            int x; scanf("%d", &x); x--;
            matrix[i][x] = (matrix[i][x] + 1) % 7;
        }
    }
    int rank = 0;
    for (int j = 0; j < N; j++) {
        // try to get a 1 at position (j, j)
        int row = -1;
        for (int i = rank; i < M; i++) {
            if (matrix[i][j]) {
                row = i;
                break;
            }
        }
        if (row < 0) continue;
        // swap the 1 into the current row
        swap(matrix[rank], matrix[row]);
        int c = inv[matrix[rank][j]];
        for (int k = j; k <= N; k++) {
            matrix[rank][k] = (c * matrix[rank][k]) % 7;
        }
        // zero out the entries above and below
        for (int i = 0; i < M; i++) {
            if (i == rank) continue;
            int c = 6 * matrix[i][j];
            for (int k = j; k <= N; k++) {
                matrix[i][k] = (matrix[i][k] + c * matrix[rank][k]) % 7;
            }
        }
        ++rank;
    }
    for (int i = rank; i < M; i++) {
        if (matrix[i][N]) {
            puts("Inconsistent data.");
            return;
        }
    }
    if (rank < N) {
        puts("Multiple solutions.");
        return;
    }
    for (int i = 0; i < N; i++) {
        if (i > 0) putchar(' ');
        printf("%d", sol[matrix[i][N]]);
    }
    putchar('\n');
}
int main() {
    for(;;) {
        int N, M; scanf("%d %d", &N, &M);
        if (M == 0 && N == 0) return 0;
        do_testcase(N, M);
    }
}
