// 2026-05-18
// It's often possible to find a valid path whose length is the Manhattan
// distance between the two given squares.  By staring at a Sierpinski carpet
// for long enough you may manage to convince yourself of the following: the
// distance between the two given squares is the Manhattan distance *unless*
// there exists a nonempty set S of removed regions such that every path whose
// length is the Manhattan distance passes through *all* members of the set.
// In that case, let R be the largest region in S, and if there are multiple
// largest regions in S, then the one closest to the first point.  The optimal
// distance is obtained by choosing an appropriate corner of S as an
// intermediate point (so the distance is the Manhattan distance from the first
// point to the chosen corner plus the Manhattan distance from the chosen
// corner to the second point).  We can simply try both possible choices for
// that corner.  (All this will make intuitive sense if you actually look at
// the diagram and imagine some possible choices of inputs.)
//
// NOTE: the order in which the coordinates are given on SPOJ is different from
// the order in official test data, which, according to the problem description
// on the IPSC website, is H_c, T_c, H_r, T_r.  But interestingly, the sample
// test case on the IPSC website gives the coordinates in SPOJ order.  Not sure
// what happened here.
#include <iostream>
#include <stdlib.h>
#include <utility>
using namespace std;
long long pow3[39];
long long do_testcase() {
    int D; cin >> D;
    long long x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
    --x1; --y1; --x2; --y2;
    for (int e = D - 1; e >= 0; e--) {
        int X1 = x1/pow3[e], X2 = x2/pow3[e], Y1 = y1/pow3[e], Y2 = y2/pow3[e];
        if (X1 != X2 && Y1 != Y2) {
            return abs(x1 - x2) + abs(y1 - y2);
        }
        if (X1 == X2) {
            if (Y1 == Y2) {
                x1 %= pow3[e];
                y1 %= pow3[e];
                x2 %= pow3[e];
                y2 %= pow3[e];
                continue;
            } else {
                swap(x1, y1);
                swap(x2, y2);
                swap(X1, Y1);
                swap(X2, Y2);
            }
        }
        // now Y1 == Y2 and X1 != X2
        if (x1 > x2) {
            swap(x1, x2);
            swap(X1, X2);
        }
        // now Y1 == Y2 and x1 < x2
        long long x3;
        long long x4;
        for (; e >= 0; e--) {
            // check whether there's a black square of size `e` that lies on all
            // paths whose length is the Manhattan distance
            if (y1 % (3*pow3[e]) / pow3[e] != 1 ||
                y2 % (3*pow3[e]) / pow3[e] != 1) {
                continue;
            }
            x3 = x1/pow3[e];
            x4 = x2/pow3[e];
            if (x4 - x3 >= 2) break;
        }
        if (e < 0) return abs(x2 - x1) + abs(y2 - y1);
        // (cx, cy1) is immediately above and to the left of the top-left corner
        // of the leftmost obstructing square; (cx, cy2) is immediately below
        // and to the left of the bottom-left corner
        const long long cx = (x3 % 3 == 0)
                             ? (x3 + 1)*pow3[e] - 1
                             : (x3 + 2)*pow3[e] - 1;
        const long long cy1 = pow3[e]*(y1/pow3[e]) - 1;
        const long long cy2 = cy1 + pow3[e] + 1;
        return abs(x1 - cx) + abs(cx - x2) + min(abs(y1 - cy1) + abs(cy1 - y2),
                                                 abs(y1 - cy2) + abs(cy2 - y2));
    }
    return 0;  // identical squares
}
int main() {
    pow3[0] = 1;
    for (int i = 1; i <= 38; i++) pow3[i] = 3*pow3[i - 1];
    int T; cin >> T; while (T--) cout << do_testcase() << '\n';
}
