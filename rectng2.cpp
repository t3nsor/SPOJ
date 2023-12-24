// 2023-12-24
#include <stdio.h>
#include <string.h>
struct Rect {
    // The `v` and `h` arrays give the status of the right and bottom edge,
    // respectively, of cell (i, j) (where the upper-left cell is (0, 0)). For
    // the input rectangles and the infimum, this is just 1 if the edge is
    // present, and 0 if absent. For the supremum, the meaning of the value is
    // given by the `Status` enum, below.
    int v[30][30];
    int h[30][30];
};
enum Status {
    UNKNOWN = 0,
    LTVALID = 1,
    RBVALID = 2,
    VALID = 3,
    ABSENT = 4,
};
void gets_discard() {
    int c;
    do {
        c = getchar();
    } while (c != '\n');
}
void do_testcase(int R, int C) {
    gets_discard();
    Rect r1, r2, inf, sup;
    for (int i = 0; i < R; i++) {
        getchar();
        for (int j = 0; j < C; j++) {
            r1.h[i][j] = (getchar() == '_');
            r1.v[i][j] = (getchar() == '|');
        }
        getchar();
        getchar();
        for (int j = 0; j < C; j++) {
            r2.h[i][j] = (getchar() == '_');
            r2.v[i][j] = (getchar() == '|');
        }
        gets_discard();
    }

    // The infimum is always just the union. On the other hand, the supremum can
    // have edges that are not in the intersection (see sample test data).
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            inf.h[i][j] = r1.h[i][j] || r2.h[i][j];
            inf.v[i][j] = r1.v[i][j] || r2.v[i][j];
            // set sup edges as initially unvalidated (except of course for the
            // edges that belong to the whole rectangle)
            if (i == R - 1) {
                sup.h[i][j] = VALID;
            } else {
                sup.h[i][j] = (r1.h[i][j] && r2.h[i][j] ? UNKNOWN : ABSENT);
            }
            if (j == C - 1) {
                sup.v[i][j] = VALID;
            } else {
                sup.v[i][j] = (r1.v[i][j] && r2.v[i][j] ? UNKNOWN : ABSENT);
            }
        }
    }

    // The naive solution of repeatedly removing any edges that are not "anchored"
    // at both ends fails on cases such as
    //  _ _ _   _ _ _
    // | |_|_| | |_ _|
    // |_|_| | |_|_| |
    // |_ _|_| |_|_|_|
    //
    // where the intersection is
    //  _ _ _ 
    // | |_ _|
    // |_|_| |
    // |_ _|_|
    //
    // and all edges are anchored at both ends, but with circular dependencies.
    // We need to validate edges from the outside-in instead. The idea is that
    // a horizontal line segment (consisting of one or more horizontal edges) is
    // validated if both of its endpoints are touching vertical line segments
    // that were previously validated, and vice versa. However, to avoid having
    // to deal with multi-edge segments, we instead just consider each edge in
    // isolation and keep track of whether it's "left-validated" and
    // "right-validated" (or "top-validated" and "bottom-validated" in the case
    // of a vertical edge). The leftmost edge in a horizontal segment that
    // should be validated becomes left-validated first, then we can
    // left-validate the horizontal edge to its right, and so on. We terminate
    // when no further edge can be further validated at either end. This isn't
    // very efficient, but the test data size is really small.

top:
    // attempt to validate horizontal edges
    for (int i = 0; i < R - 1; i++) {
        for (int j = 0; j < C; j++) {
            if (sup.h[i][j] == ABSENT) continue;
            if (!(sup.h[i][j] & LTVALID)) {
                // Check whether we can validate this edge at the left. This
                // means its left endpoint is touching a left-validated edge or
                // two fully validated vertical edges.
                if (j == 0 || (sup.h[i][j - 1] & LTVALID) ||
                    (sup.v[i][j - 1] == VALID &&
                     sup.v[i + 1][j - 1] == VALID)) {
                    sup.h[i][j] |= LTVALID;
                    goto top;
                }
            }
            if (!(sup.h[i][j] & RBVALID)) {
                // Analogously, try to validate this edge at the right.
                if (j == C - 1 || (sup.h[i][j + 1] & RBVALID) ||
                    (sup.v[i][j] == VALID && sup.v[i + 1][j] == VALID)) {
                    sup.h[i][j] |= RBVALID;
                    goto top;
                }
            }
        }
    }
    // attempt to validate vertical edges; very similar
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C - 1; j++) {
            if (sup.v[i][j] == ABSENT) continue;
            if (!(sup.v[i][j] & LTVALID)) {
                if (i == 0 || (sup.v[i - 1][j] & LTVALID) ||
                    (sup.h[i - 1][j] == VALID &&
                     sup.h[i - 1][j + 1] == VALID)) {
                    sup.v[i][j] |= LTVALID;
                    goto top;
                }
            }
            if (!(sup.v[i][j] & RBVALID)) {
                if (i == R - 1 || (sup.v[i + 1][j] & RBVALID) ||
                    (sup.h[i][j] == VALID &&
                     sup.h[i][j + 1] == VALID)) {
                    sup.v[i][j] |= RBVALID;
                    goto top;
                }
            }
        }
    }

    // print infimum and supremum
    putchar(' ');
    for (int i = 0; i < C; i++) {
        putchar('_');
        putchar(' ');
    }
    putchar(' ');
    putchar(' ');
    for (int i = 0; i < C; i++) {
        putchar('_');
        putchar(' ');
    }
    putchar('\n');
    for (int i = 0; i < R; i++) {
        putchar('|');
        for (int j = 0; j < C; j++) {
            putchar(inf.h[i][j] ? '_' : ' ');
            putchar(inf.v[i][j] ? '|' : ' ');
        }
        putchar(' ');
        putchar('|');
        for (int j = 0; j < C; j++) {
            putchar(sup.h[i][j] == VALID ? '_' : ' ');
            putchar(sup.v[i][j] == VALID ? '|' : ' ');
        }
        putchar('\n');
    }
}
int main() {
    int casenum = 0;
    for (;;) {
        casenum++;
        int R, C; scanf("%d %d", &C, &R);
        if (R == 0) return 0;
        printf("Case %d:\n", casenum);
        gets_discard();
        do_testcase(R, C);
        putchar('\n');
    }
}
