// 2025-06-29
// Lazy propagation can't be done efficiently in two dimensions (or more than
// two). However, this is a case where we don't actually need to propagate. In
// the one-dimensional version of the problem, each update can simply "sit" at
// one or more basic intervals. To be more precise, consider each update to be
// made up of a set of maximal basic updates. Each node in the tree,
// representing basic interval [a, b), stores two values:
// * The maximum basic update that has been done to [a, b) or to any of its
//   subintervals (*not* including basic updates to any intervals that contain
//   [a, b))
// * The maximum basic update that has been done to [a, b) so far (*not*
//   including basic updates to any intervals that contain or are contained by
//   [a, b))
// To perform an update, we update the first and second values for the nodes
// corresponding to the basic updates, and then update the first values for the
// ancestors of those nodes. To perform a query, we walk down the segment tree
// as usual, taking the maximum of the first values of all nodes we visit whose
// intervals are contained within the query interval (as usual, not descending
// past those nodes) and the second values of all nodes we visit.
//
// In the 2d version, we have to maintain a set of basic rectangles (a Cartesian
// product of two 1d segment trees). Certainly each basic rectangle will have to
// contain the computed maximum of all basic updates performed to either itself
// or any basic rectangles contained within it, but not only can a basic
// rectangle be affected by basic updates that strictly contain it as in the 1d
// case, but also by basic updates that strictly contain it along one dimension
// and are strictly contained by it along the other, e.g., the maximum value on
// [0, 2) x [0, 2) is affected by a basic update on [0, 1) x [0, 4). For this
// reason we have to maintain 4 pieces of information per basic rectangle
// [a, b) x [c, d):
// 1. The maximum basic update that has been done to this rectangle or to any
//    rectangles contained by it,
// 2. The maximum basic update that has been done to [a', b') x [c, d) where
//    [a', b') is a (non-strict) subset of [a, b),
// 3. The maximum basic update that has been done to [a, b) x [c', d') where
//    [c', d') is a (non-strict) subset of [c, d),
// 4. The maximum basic update that has been done to this rectangle.
// For each update [a, b) x [c, d) there are only O(log^2 N) basic rectangles
// that need updating, namely the Cartesian product of the basic intervals
// encountered while descending 1d trees for [a, b) and [c, d). For a query
// [a, b) x [c, d), we visit the nodes corresponding to the same Cartesian
// product and taking into account the first value of each basic rectangle
// that's contained within the query rectangle, the second or third value of
// each basic rectangle [a', b') x [c', d') where [a', b') is contained within
// [a, b) or [c', d') is contained within [c, d) (respectively), and the fourth
// value of each basic rectangle that we visit.
//
// This isn't as complicated as it sounds. Refer to the code.
#include <algorithm>
#include <iostream>
using namespace std;
struct Node { int max; int max1; int max2; int upd; };
Node tree[2048][2048];
int X, Y;
int query1(int n1, bool base1, int n2, int tl2, int tr2, int ql2, int qr2) {
    int result = 0;
    if (tl2 >= ql2 && tr2 <= qr2) {
        if (base1) result = max(result, tree[n1][n2].max);
        else result = max(result, tree[n1][n2].max2);
    } else {
        if (base1) result = max(result, tree[n1][n2].max1);
        else result = max(result, tree[n1][n2].upd);
        const int tm2 = (tl2 + tr2) / 2;
        if (ql2 < tm2 && qr2 > tl2) {
            result = max(result, query1(n1, base1, 2*n2, tl2, tm2, ql2, qr2));
        }
        if (ql2 < tr2 && qr2 > tm2) {
            result = max(result,
                         query1(n1, base1, 2*n2 + 1, tm2, tr2, ql2, qr2));
        }
    }
    return result;
}
int query(int n1, int tl1, int tr1, int ql1, int qr1, int ql2, int qr2) {
    const bool base1 = (tl1 >= ql1 && tr1 <= qr1);
    int result = query1(n1, base1, 1, 0, Y, ql2, qr2);
    if (!base1) {
        const int tm1 = (tl1 + tr1) / 2;
        if (ql1 < tm1 && qr1 > tl1) {
            result = max(result, query(2*n1, tl1, tm1, ql1, qr1, ql2, qr2));
        }
        if (ql1 < tr1 && qr1 > tm1) {
            result = max(result, query(2*n1 + 1, tm1, tr1, ql1, qr1, ql2, qr2));
        }
    }
    return result;
}
void update1(int n1, bool base1,
             int n2, int tl2, int tr2, int ul2, int ur2, int h) {
    if (tl2 >= ul2 && tr2 <= ur2) {
        if (base1) {
            tree[n1][n2].upd = h;
            tree[n1][n2].max1 = max(tree[n1][n2].max1, h);
            tree[n1][n2].max2 = max(tree[n1][n2].max2, h);
            tree[n1][n2].max = max(tree[n1][n2].max, h);
        }
    } else {
        const int tm2 = (tl2 + tr2) / 2;
        if (ul2 < tm2 && ur2 > tl2) {
            update1(n1, base1, 2*n2, tl2, tm2, ul2, ur2, h);
        }
        if (ul2 < tr2 && ur2 > tm2) {
            update1(n1, base1, 2*n2 + 1, tm2, tr2, ul2, ur2, h);
        }
        tree[n1][n2].max = max(tree[n1][n2].max, tree[n1][2*n2].max);
        tree[n1][n2].max = max(tree[n1][n2].max, tree[n1][2*n2 + 1].max);
        tree[n1][n2].max2 = max(tree[n1][n2].max2, tree[n1][2*n2].max2);
        tree[n1][n2].max2 = max(tree[n1][n2].max2, tree[n1][2*n2 + 1].max2);
    }
    if (!base1) {
        tree[n1][n2].max1 = max(tree[n1][n2].max1, tree[2*n1][n2].max1);
        tree[n1][n2].max1 = max(tree[n1][n2].max1, tree[2*n1 + 1][n2].max1);
        tree[n1][n2].max = max(tree[n1][n2].max, tree[2*n1][n2].max);
        tree[n1][n2].max = max(tree[n1][n2].max, tree[2*n1 + 1][n2].max);
    }
}
void update(int n1, int tl1, int tr1, int ul1, int ur1, int ul2, int ur2,
            int h) {
    const bool base1 = (tl1 >= ul1 && tr1 <= ur1);
    if (!base1) {
        const int tm1 = (tl1 + tr1) / 2;
        if (ul1 < tm1 && ur1 > tl1) {
            update(2*n1, tl1, tm1, ul1, ur1, ul2, ur2, h);
        }
        if (ul1 < tr1 && ur1 > tm1) {
            update(2*n1 + 1, tm1, tr1, ul1, ur1, ul2, ur2, h);
        }
    }
    update1(n1, base1, 1, 0, Y, ul2, ur2, h);
}
int main() {
    ios::sync_with_stdio(false);
    int N; cin >> X >> Y >> N;
    while (N--) {
        int dx, dy, h, x, y; cin >> dx >> dy >> h >> x >> y;
        int z = query(1, 0, X, x, x + dx, y, y + dy);
        update(1, 0, X, x, x + dx, y, y + dy, z + h);
    }
    cout << tree[1][1].max << '\n';
}
