// 2023-10-23
#include <iostream>
#include <cstring>
#include <vector>
using namespace std;
constexpr int inv[7]={0, 1, 4, 5, 2, 3, 6};
constexpr int sol[7]={7, 8, 9, 3, 4, 5, 6};
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
