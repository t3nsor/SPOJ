// 2024-01-07
// As an obvious special case, if any edge of the NP is slanted, then the answer
// is "NO". Otherwise, Because all plots are disjoint, all we have to do is
// determine which plots lie fully inside the national park, and compare the
// sum of their areas with the area of the national park.
//
// A plot lies inside the NP if
// * no point on the boundary of the NP lies in the interior of the plot, and
// * any point in the interior of the plot lies in the NP. We can test this by
//   casting a ray. To ensure that the ray doesn't hit any corner of NP, we scale
//   up all coordinates of the NP and the plots by 2, and then choose an interior
//   point with odd coordinates.
//
// Note: The test data do not include tricky cases such as
// * rectangles of area 0
// * vertices of NP that lie on the line segment between two other vertices
// and the maximum absolute value in the test data is 10,000.
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <vector>
using namespace std;
bool do_testcase(int N, int M) {
    vector<int> NPx(N), NPy(N);
    for (int i = 0; i < N; i++) {
        scanf("%d %d", &NPx[i], &NPy[i]);
        NPx[i] *= 2; NPy[i] *= 2;
    }
    vector<int> Px1(M), Py1(M), Px2(M), Py2(M);
    for (int i = 0; i < M; i++) {
        scanf("%d %d %d %d", &Px1[i], &Py1[i], &Px2[i], &Py2[i]);
        Px1[i] *= 2; Py1[i] *= 2;
        Px2[i] *= 2; Py2[i] *= 2;
        if (Px1[i] > Px2[i]) swap(Px1[i], Px2[i]);
        if (Py1[i] > Py2[i]) swap(Py1[i], Py2[i]);
    }
    long long NParea = 0;
    for (int i = 0; i < N; i++) {
        const int x1 = NPx[i];
        const int y1 = NPy[i];
        const int x2 = NPx[(i + 1) % N];
        const int y2 = NPy[(i + 1) % N];
        if (x1 != x2 && y1 != y2) {
            // slanted edge
            return false;
        }
        NParea += x1 * y2 - x2 * y1;
    }
    NParea = abs(NParea) / 2;
    long long rectarea = 0;
    for (int i = 0; i < M; i++) {
        bool fail = false;
        // Consider the ray from (-inf, yint) to (xint, yint). How many edges of
        int cnt = 0;
        const int xint = Px1[i] + 1;
        const int yint = Py1[i] + 1;
        for (int j = 0; j < N; j++) {
            int npx1 = NPx[j];
            int npy1 = NPy[j];
            int npx2 = NPx[(j + 1) % N];
            int npy2 = NPy[(j + 1) % N];
            if (npx1 == npx2) {
                // vertical edge of NP
                if (npy1 > npy2) swap(npy1, npy2);
                if (npx1 > Px1[i] && npx1 < Px2[i] &&
                    npy1 < Py2[i] && npy2 > Py1[i]) {
                    fail = true;
                    break;
                }
                // the edge might intersect the ray
                if (npx1 < xint && npy1 < yint && npy2 > yint) ++cnt;
            } else {
                // horizontal edge of NP
                if (npx1 > npx2) swap(npx1, npx2);
                if (npy1 > Py1[i] && npy1 < Py2[i] &&
                    npx1 < Px2[i] && npx2 > Px1[i]) {
                    fail = true;
                    break;
                }
            }
        }
        if (!fail && (cnt % 2 == 1)) {
            rectarea += (Px2[i] - Px1[i]) * (Py2[i] - Py1[i]);
        }
    }
    return NParea == rectarea;
}
int main() {
    int N, M;
    for (;;) {
        scanf("%d %d", &N, &M);
        if (N == 0) return 0;
        puts(do_testcase(N, M) ? "YES" : "NO");
    }
}
