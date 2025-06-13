// 2025-06-13
// The problem statement on SPOJ is nearly unreadable. You can find a better
// translation on DMOJ: https://dmoj.ca/problem/noi07p2
// The idea is not too hard to describe. At any given point in time your
// current state is a point in R^3 whose coordinates are the amount of A,
// amount of B, and amount of cash you have. You perform transactions to shift
// to another position in R^3. At the end of N days, you want to be at the
// highest possible point on the z axis.
//
// The basic observation is that if there is a set of points you could
// possibly be at, it is never suboptimal to be at a vertex of the convex hull
// of that set. This is because the exchange rate on each day constrains you
// during that day to move on a plane satisfying A_K x + B_K y + z = C. If you
// started in the interior of the previous day's convex hull, then you get a
// value of C on the current day that is less than optimal, which then makes
// you start with a suboptimal C on the following day and so on. Points that
// are on the line segment between two vertices of the convex hull can never
// be better than the vertices.
//
// On each day it is always possible to reach the z axis (by selling all of
// your A and B). This implies that the convex hull we need to keep track of
// consists of some points in the xy plane together with a single point on the
// z axis (the maximum cash you can have). This is what the hint is telling
// us. (So we don't really update a 3d convex hull; we update a 2d convex
// hull together with the single point on the z axis.)
//
// At the start of each day we first check whether the exchange rates allow us
// to obtain a larger amount of cash than we would have if we had held cash
// from the previous day. Basically, this means we find the point in the xy
// plane that is furthest in the direction of the vector (A_K, B_K). Then,
// using either this amount of cash (or the amount held from the previous day,
// if greater) we buy as much of A and B as we can and update the hull.
//
// The implementation is a PITA because in order to efficiently update the
// hull, we need a data structure that keeps the points sorted in order of
// their x or y coordinate or angle, but in order to efficiently query the
// best point (the one that will give us the most cash if we sell) we need to
// be able to search for the *side* of the convex hull whose angle is closest
// to being perpendicular to (A_K, B_K); the side angles depend on the
// coordinates of two adjacent points in the hull. This solution uses C++14
// transparent comparators: the `std::set` is kept sorted by x coordinate but
// with the angle of the side between a point and the next point stored as an
// ancillary piece of information; an overload of the comparator's
// `operator()` allows us to search based on the angle of the side. Prior to
// C++14, you can still do it by e.g. flipping some global state to tell the
// comparator to compare side angles instead of x coordinates.
#include <set>
#include <stdio.h>
#include <math.h>
#include <utility>
using namespace std;
// precondition x1 < x2 < x3
// returns true if (x2, y2) should **not** be in the set
bool cw(double x1, double y1, double x2, double y2, double x3, double y3) {
    return (x2 - x1) * (y2 - y3) <= (y2 - y1) * (x2 - x3);
}
struct Entry {
    double x;
    mutable double y;
    mutable double angle;
};
struct Angle { double angle; };
struct Compare {
    using is_transparent = void;
    bool operator()(const Entry& e1, const Entry& e2) const {
        return e1.x < e2.x;
    }
    bool operator()(const Entry& e, double x) const {
        return e.x < x;
    }
    bool operator()(const Entry& e, Angle a) const {
        return e.angle > a.angle;
    }
};
void do_testcase() {
    int N; scanf("%d", &N);
    double S; scanf("%lf", &S);
    set<Entry, Compare> s;
    while (N--) {
        double a, b, r; scanf("%lf %lf %lf", &a, &b, &r);
        // find optimal sale of vouchers for cash
        Angle angle = {atan2(a, -b)};
        auto it = s.lower_bound(angle);
        if (it != s.end()) {
            S = max(S, a*it->x + b*it->y);
        }
        // spend all our cash to buy vouchers (note that we can use the cash we
        // obtained from selling vouchers earlier in the day)
        const double y = S / (a*r + b);
        const double x = r*y;
        it = s.lower_bound(x);
        // check whether this point should actually be added
        if (it == s.end()) {
            // our x-coordinate is strictly greater than everything else
            it = s.insert(Entry{x, y}).first;
        } else {
            // check for exact match (unlikely)
            if (x == it->x) {
                if (y <= it->y) continue;
                it->y = y;
            } else if (it == s.begin()) {
                // we would be inserted at the very beginning
                if (y <= it->y) continue;
                it = s.insert(Entry{x, y}).first;
            } else {
                // we would be inserted between two points *it2 and *it
                const auto it2 = prev(it);
                if (cw(it2->x, it2->y, x, y, it->x, it->y)) continue;
                it = s.insert(Entry{x, y}).first;
            }
        }
        // remove points after it that are no longer on the boundary of the
        // convex hull
        auto it2 = next(it);
        while (it2 != s.end() && next(it2) != s.end()) {
            const auto it3 = next(it2);
            if (cw(x, y, it2->x, it2->y, it3->x, it3->y)) ++it2; else break;
        }
        s.erase(next(it), it2);
        // and before
        it2 = it;
        while (it2 != s.begin()) {
            const auto it3 = prev(it2);
            if (it3 == s.begin()) {
                if (it3->y <= y) it2 = it3;
                break;
            } else {
                const auto it4 = prev(it3);
                if (cw(it4->x, it4->y, it3->x, it3->y, x, y)) --it2;
                else break;
            }
        }
        s.erase(it2, it);
        // paranoia: adjust y-coordinate to avoid violating invariants
        if (next(it) != s.end() && it->y < next(it)->y) {
            fprintf(stderr, "adjusting y-coordinate\n");
            it->y = next(it)->y;
        }
        if (it != s.begin() && it->y > prev(it)->y) {
            fprintf(stderr, "adjusting y-coordinate\n");
            it->y = prev(it)->y;
        }
        // patch up angles
        if (next(it) == s.end()) {
            it->angle = atan2(1, 0);
        } else {
            it->angle = atan2(it->y - next(it)->y, it->x - next(it)->x);
        }
        if (it != s.begin()) {
            prev(it)->angle = atan2(prev(it)->y - it->y, prev(it)->x - it->x);
            // more paranoia
            if (prev(it) != s.begin() &&
                prev(it)->angle > prev(prev(it))->angle) {
                fprintf(stderr, "adjusting angle\n");
                prev(it)->angle = prev(prev(it))->angle;
            }
            if (it->angle > prev(it)->angle) {
                fprintf(stderr, "adjusting angle\n");
                it->angle = prev(it)->angle;
            }
        }
    }
    printf("%.3f\n", S);
}
int main() {
    int T; scanf("%d", &T); while (T--) do_testcase();
}
