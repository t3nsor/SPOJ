// 2024-01-06
// This problem is not as hard as it looks. Start with every cubelet occupied by
// a block, and delete a block only if the views assign contradictory colours to
// it. This works because if there is such a block B, we can't delete any other
// blocks in order to avoid having to delete B (deleting other blocks can only
// increase the constraints on B). When there are no more blocks to delete, we
// are left with the maximum number of blocks that can satisfy the constraints.
// The implementation is slightly tricky to get right.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
void do_testcase() {
    int N; cin >> N;
    vector<string> front(N), left(N), back(N), right(N), top(N), bottom(N);
    for (int i = 0; i < N; i++) {
        cin >> front[i] >> left[i] >> back[i]
            >> right[i] >> top[i] >> bottom[i];
    }
    // Convert the views into constraints that are easier to use. Index the
    // blocks of the cube by (i, j, k) where `i` is the row (from the top),
    // `j` is the column (from the left), and `k` is the depth (from the front).
    // Then
    // * cij[0][i][j] is the colour of the block at (i, j, k) where `k` is
    //   minimal
    // * cij[1][i][j] is the colour of the block at (i, j, k) where `k` is
    //   maximal
    // * cik, cjk are defined analogously
    char cij[2][10][10];
    char cik[2][10][10];
    char cjk[2][10][10];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cij[0][i][j] = front[i][j];
            cij[1][i][j] = back[i][N - j - 1];
        }
    }
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            cik[0][i][k] = left[i][N - k - 1];
            cik[1][i][k] = right[i][k];
        }
    }
    for (int j = 0; j < N; j++) {
        for (int k = 0; k < N; k++) {
            cjk[0][j][k] = top[N - k - 1][j];
            cjk[1][j][k] = bottom[k][j];
        }
    }

    bool a[10][10][10];  // whether we have a block in this position
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                if (cij[0][i][j] == '.' ||
                    cik[0][i][k] == '.' ||
                    cjk[0][j][k] == '.') {
                    a[i][j][k] = 0;
                } else {
                    a[i][j][k] = 1;
                }
            }
        }
    }

top:
    // As long as there's a block that is given contradictory colours by the
    // constraints, remove that block.

    // First, precompute the first and last occupied coordinate for each
    // possible fixed (i, j), (i, k), or (j, k) pair.
    int mij[10][10], Mij[10][10];
    int mik[10][10], Mik[10][10];
    int mjk[10][10], Mjk[10][10];
    for (int p = 0; p < N; p++) {
        for (int q = 0; q < N; q++) {
            mij[p][q] = mik[p][q] = mjk[p][q] = 100;
            Mij[p][q] = Mik[p][q] = Mjk[p][q] = -100;
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                if (!a[i][j][k]) continue;
                mij[i][j] = min(mij[i][j], k);
                Mij[i][j] = max(Mij[i][j], k);
                mik[i][k] = min(mik[i][k], j);
                Mik[i][k] = max(Mik[i][k], j);
                mjk[j][k] = min(mjk[j][k], i);
                Mjk[j][k] = max(Mjk[j][k], i);
            }
        }
    }

    // Now just iterate through all blocks and check whether the block is given
    // contradictory colours.
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                if (!a[i][j][k]) continue;
                char constraints[6] = {0};
                if (k == mij[i][j]) {
                    constraints[0] = cij[0][i][j];
                }
                if (k == Mij[i][j]) {
                    constraints[1] = cij[1][i][j];
                }
                if (j == mik[i][k]) {
                    constraints[2] = cik[0][i][k];
                }
                if (j == Mik[i][k]) {
                    constraints[3] = cik[1][i][k];
                }
                if (i == mjk[j][k]) {
                    constraints[4] = cjk[0][j][k];
                }
                if (i == Mjk[j][k]) {
                    constraints[5] = cjk[1][j][k];
                }
                char current_constraint = 0;
                for (const int constraint : constraints) {
                    if (constraint == 0) continue;
                    if (current_constraint == 0) {
                        current_constraint = constraint;
                    } else {
                        if (constraint != current_constraint) {
                            // contradictory constraints; remove the block and
                            // repeat as many times as necessary.
                            a[i][j][k] = 0;
                            goto top;
                        }
                    }
                }
            }
        }
    }

    int result = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                if (a[i][j][k]) ++result;
            }
        }
    }
    printf("%d\n", result);
}
int main() {
    int t; cin >> t;
    while (t--) do_testcase();
}
