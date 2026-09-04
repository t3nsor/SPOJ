// 2026-09-04
// This is an implementation of the O(KN^3) algorithm from "Finding Minimum Area
// k-gons" by Eppstein et al. (1992) (the space complexity can be optimized to
// O(N^2), but I am lazy and did not do this).  The algorithm is described in
// more detail in the code below.  The remainder of this section discusses an
// alternative approach (the implementation of which is left as an exercise).
//
// There are two official solutions for this problem, which can be found here:
// https://infos.infosbg.com/files/Contests/NOI-3/2008/day1/AB/problems/poly.zip
// One of them is a DP (I haven't closely analyzed whether it uses Eppstein's
// algorithm) while the other, curiously, uses recursive backtracking.  The idea
// is to build up the polygon one vertex at a time in counterclockwise order.  A
// particular point P can be selected only if ABP is a counterclockwise turn,
// where A and B are the previous two points.  Also, a point cannot be selected
// if selecting that point would require a clockwise turn to close the polygon.
// And the most crucial insight is this: each time a new vertex is added, it
// contributes a new triangle to the area, and this partial area is a lower
// bound for the final area of the polygon when completed, so we can return (not
// descend further) if partial area is already greater than or equal to the
// minimal area for a complete convex K-gon found earlier in the search.  This
// optimization is apparently quite powerful: ChatGPT estimates that it gives
// an average-case complexity of O(N^2 polylog(N)) and it's not obvious how to
// construct a case that would make it take something like N^{K-c} steps.

#include <algorithm>
#include <math.h>
#include <stdio.h>
using namespace std;

const double pi = acos(-1.0);

// dp[i][j][k][m] stores twice the minimum area of a convex polygon with m+2
// vertices given the following constraints:
// * point i is the vertex with the least y-coordinate in the polygon, breaking
//   ties by x-coordinate.  (I don't think it actually depends which way you
//   break ties: choosing the leftmost (resp. rightmost) point is equivalent to
//   an infinitesimal positive (resp. negative) rotation.)
// * point j is the vertex immediately following point i in counterclockwise
//   order
// * k != i, k != j, point k is not necessarily a vertex, and all vertices
//   other than j and possibly k lie on the same side of the line between points
//   j and k.
int dp[49][49][49][9];

int X[49], Y[49];

int order[49][48];

int det(int i, int j, int k) {
    const int dx1 = X[j] - X[i];
    const int dy1 = Y[j] - Y[i];
    const int dx2 = X[k] - X[j];
    const int dy2 = Y[k] - Y[j];
    return dx1*dy2 - dx2*dy1;
}

int main() {
    int N, K; scanf("%d %d", &N, &K);
    if (K < 3) { puts("0"); return 0; }
    for (int i = 0; i < N; i++) scanf("%d %d", X + i, Y + i);
    // For each point `i`, make a list of other points `j` sorted in
    // counterclockwise order of the lines between `i` and `j`.  Note: lines,
    // not rays.  We can arbitrarily choose the orientation that's considered
    // minimal (we choose horizontal).
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (j == i) continue;
            if (j < i) order[i][j] = j; else order[i][j - 1] = j;
        }
        sort(order[i], order[i] + N - 1, [i](int j, int k) {
                 // atan2 is normally in (-pi, +pi]
                 double aj = atan2(Y[j] - Y[i], X[j] - X[i]);
                 double ak = atan2(Y[k] - Y[i], X[k] - X[i]);
                 if (aj < 0) aj += pi; if (Y[j] == Y[i]) aj = 0;
                 if (ak < 0) ak += pi; if (Y[k] == Y[i]) ak = 0;
                 return aj < ak;
             });
    }

    int result = 1e9;
    for (int m = 1; m <= K - 2; m++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++) {
                    dp[i][j][k][m] = 1e9;
                }
            }
            for (int j = 0; j < N; j++) {
                if (Y[j] < Y[i] || (Y[j] == Y[i] && X[j] <= X[i])) continue;
                int ma = 1e9;
                // find point i in the list of points around j
                int ipos;
                for (int k = 0; k < N - 1; k++) {
                    if (order[j][k] == i) { ipos = k; break; }
                }
                // process points in *clockwise* order starting right after `i`
                for (int d = 1; d < N - 1; d++) {
                    const int k = order[j][(ipos - d + (N - 1)) % (N - 1)];
                    const int D = det(i, j, k);
                    if (D > 0 &&
                        (Y[k] > Y[i] || (Y[k] == Y[i] && X[k] > X[i]))) {
                        // point `k` is to the LEFT of the ray from point `i` to
                        // `j`: we could possibly construct a polygon using the
                        // triangle i, j, k and a polygon with one fewer side
                        // whose first two vertices are i, k.
                        // (Note that the additional condition that point `k`
                        // must be above point `i` is not directly stated in the
                        // paper, but the correctness of the algorithm depends
                        // on it: if `k` is below `i` then the triangle ijk
                        // can't be part of the polygon with `i` being the
                        // vertex of minimal y-coordinate, and if `i` doesn't
                        // have the minimal y-coordinate then convexity can be
                        // violated.)
                        ma = min(ma, dp[i][k][j][m - 1] + D);
                    }
                    dp[i][j][k][m] = ma;
                    if (m == K - 2) result = min(result, dp[i][j][k][m]);
                }
            }
        }
    }
    printf("%d\n", (result >= 1e9 ? 0 : result / 2));
}
