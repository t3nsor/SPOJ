// 2023-12-16
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <math.h>
using namespace std;
template <class T>
struct Point {
    T x;
    T y;
    Point() = default;
    Point(T x, T y) : x(x), y(y) {}
    Point(const Point<int>& p) : x(p.x), y(p.y) {}
    friend Point operator+(Point p1, Point p2) {
        return {p1.x + p2.x, p1.y + p2.y};
    }
    friend Point operator-(Point p1, Point p2) {
        return {p1.x - p2.x, p1.y - p2.y};
    }
    T sqrdist() const {
        return x*x + y*y;
    }
};

template <class T>
Point<T> operator*(T k, Point<T> p) {
    return {k * p.x, k * p.y};
}

bool operator==(Point<int> p1, Point<int> p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

struct Circle {
    Point<int> o;
    int r;
};

double dist(Point<double> p1, Point<double> p2) {
    return hypot(p1.x - p2.x, p1.y - p2.y);
}
int get_circle(Point<double> p, Circle c1, Circle c2) {
    const double dist1 = fabs(dist(p, c1.o) - c1.r);
    const double dist2 = fabs(dist(p, c2.o) - c2.r);
    if (dist1 < dist2) return 1; else return 2;
}
double arcdist(Point<double> p1, Point<double> p2, Circle c) {
    if (c.r == 0) return 0.0;
    const Point<double> o = c.o;
    const Point<double> v1 = p1 - c.o;
    const Point<double> v2 = p2 - c.o;
    const double a1 = atan2(v1.y, v1.x);
    const double a2 = atan2(v2.y, v2.x);
    return c.r * min(fabs(a1 - a2), 2.0*M_PI - fabs(a1 - a2));
}
void find_intersection(Circle c1, Circle c2,
                       Point<double>& i1, Point<double>& i2) {
    const double d = dist(c1.o, c2.o);
    const double h = max((c1.r * c1.r - c2.r * c2.r + d*d) / (2*d), 0.0);
    const Point<double> D = c2.o - c1.o;
    const Point<double> q = c1.o + (h / d) * D;
    const double k = sqrt(max(c1.r * c1.r - h * h, 0.0));
    i1.x = q.x + k * D.y / d;
    i1.y = q.y - k * D.x / d;
    i2.x = q.x - k * D.y / d;
    i2.y = q.y + k * D.x / d;
}
double do_testcase() {
    Circle c1, c2;
    cin >> c1.o.x >> c1.o.y >> c1.r;
    cin >> c2.o.x >> c2.o.y >> c2.r;
    Point<double> p1, p2;
    cin >> p1.x >> p1.y;
    cin >> p2.x >> p2.y;
    const int C1 = get_circle(p1, c1, c2);
    const int C2 = get_circle(p2, c1, c2);
    // const double d = dist(p1, p2);
    // classify circles
    if (c1.o == c2.o) {
        if (c1.r == c2.r) {
            // case 1: circles coincide
            return arcdist(p1, p2, c1);
        } else {
            // case 2: circles are disjoint
            if (C1 != C2) {
                return -1;
            } else {
                return arcdist(p1, p2, C1 == 1 ? c1 : c2);
            }
        }
    }
    // case 3: r1 + r2 < distance between centers or
    //        |r1 - r2| > distance between centers
    // In both cases the circles don't intersect.
    if ((c1.r + c2.r) * (c1.r + c2.r) < (c1.o - c2.o).sqrdist() ||
        (c1.r - c2.r) * (c1.r - c2.r) > (c1.o - c2.o).sqrdist()) {
        if (C1 != C2) {
            return -1;
        } else {
            return arcdist(p1, p2, C1 == 1 ? c1 : c2);
        }
    }
    // case 4: r1 + r2 = distance between centers OR
    //         |r1 - r2| = distance between centers
    // In the first subcase we get external tangency, in the second case
    // internal tangency but these cases are very similar.
    const bool externally_tangent =
        (c1.r + c2.r) * (c1.r + c2.r) == (c1.o - c2.o).sqrdist();
    const bool internally_tangent =
        (c1.r - c2.r) * (c1.r - c2.r) == (c1.o - c2.o).sqrdist();
    if (externally_tangent || internally_tangent) {
        // find intersection point
        const Point<double> D = c2.o - c1.o;
        Point<double> i;
        if (externally_tangent) {
            i = c1.o + (c1.r / double(c1.r + c2.r)) * D;
        } else {
            i = c1.o + (c1.r / double(c1.r - c2.r)) * D;
        }
        if (C1 == 1) {
            if (C2 == 1) {
                return arcdist(p1, p2, c1);
            } else {
                return arcdist(p1, i, c1) + arcdist(p2, i, c2);
            }
        } else {
            if (C2 == 1) {
                return arcdist(p1, i, c2) + arcdist(p2, i, c1);
            } else {
                return arcdist(p1, p2, c2);
            }
        }
    }
    // case 5: 2 intersection points
    Point<double> i1, i2;
    find_intersection(c1, c2, i1, i2);
    if (C1 == C2) {
        Circle& c = C1 == 1 ? c1 : c2;
        return min(arcdist(p1, p2, c),
                   min(arcdist(p1, i1, c) + arcdist(p2, i2, c),
                       arcdist(p1, i2, c) + arcdist(p2, i1, c)) +
                   arcdist(i1, i2, c1.r > c2.r ? c1 : c2));
    } else {
        if (C1 == 2) {
            swap(c1, c2);
        }
        return min(arcdist(p1, i1, c1) + arcdist(p2, i1, c2),
                   arcdist(p1, i2, c1) + arcdist(p2, i2, c2));
    }
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        const double result = do_testcase();
        if (result < 0) {
            cout << "-1\n";
        } else {
            cout << fixed << setprecision(4) << result << '\n';
        }
    }
}
