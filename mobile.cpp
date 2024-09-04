// 2024-09-03
// The rules for mobile validity are simple:
// * dist[i][i] = 0 for all i
// * dist[i][j] = -dist[j][i] for all i, j
// * for all distinct i, j, k, consider the values dist[i][j], dist[i][k], and
//   dist[j][k]; the maximum of these values must occur 2 or 3 times among them,
//   not once. This follows from the fact that if one of i, j, k is "less
//   related" to the other two (e.g. in the diagram in the problem description,
//   4 is less related to 1 and 2 than 1 and 2 are to each other), for example
//   i, then dist[i][j] and dist[i][k] must be equal to each other and greater
//   than dist[j][k]. If none of the 3 is less related, all three distances are
//   equal.
// Checking the last condition takes O(N^3) time, which seems like it will be
// too slow, but it can be sped up by a factor of 32 by using bit operations.
// I'm not sure if there's an O(N^2) solution, but I will post one if I discover
// or hear of one.
#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
bool do_testcase(int N) {
    vector<vector<int>> matrix(N, vector<int>(N));
    vector<pair<int, int>> occ[2048];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &matrix[i][j]);
            if (matrix[i][j] >= 2048) throw;
            occ[matrix[i][j]].emplace_back(i, j);
        }
    }
    for (int i = 0; i < N; i++) {
        if (matrix[i][i] != 0) return false;
        for (int j = 0; j < i; j++) {
            if (matrix[i][j] != matrix[j][i]) return false;
        }
    }
    vector<vector<unsigned>> rows(N, vector<unsigned>((N + 31) / 32));
    vector<vector<unsigned>> cols(N, vector<unsigned>((N + 31) / 32));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            rows[i][j / 32] |= (1U << (j % 32));
            cols[j][i / 32] |= (1U << (i % 32));
        }
    }
    for (int i = 2047; i > 0; i--) {
        for (const auto& rc : occ[i]) {
            rows[rc.first][rc.second / 32] &= ~(1U << (rc.second % 32));
            cols[rc.second][rc.first / 32] &= ~(1U << (rc.first % 32));
        }
        for (const auto& rc : occ[i]) {
            for (int j = 0; j < (N + 31) / 32; j++) {
                if (rows[rc.first][j] & cols[rc.second][j]) return false;
            }
        }
    }
    return true;
}
int main() {
    int N;
    for (;;) {
        scanf("%d", &N);
        if (N == 0) break;
        puts(do_testcase(N) ? "true" : "false");
    }
}
