// 2025-08-04
// The problem is to find the area of the intersection of a rectangle with a
// union of disks.  The disks are not all the same size, but this doesn't make
// the problem any harder.  Because there are only up to 200 disks, the
// following algorithm suffices: find all interesting x-coordinates, which are
// defined as the boundaries of the rectangle (0 and X), the leftmost and
// rightmost points of each disk if they fall within the rectangle, and any
// intersection points between circles.  There are at most O(n^2) such
// interesting x-coordinates.  Between any consecutive pair thereof, no two
// circles intersect, and there is a well-defined ordering of circular arcs by
// y-coordinate (where the upper and lower halves of a circle are considered
// two separate arcs).  A sweep by y-coordinate then finds the arcs (or
// horizontal segments) that bound all subsets of the desired area, and the
// area of each such subset is found by integration.  The total time is O(n^3
// log n) because a sort is required in the inner loop.  It is possible to
// improve this to O(n^3) by updating, instead of recalculating, the order of
// arcs every time an interesting point is crossed, but this seems annoying and
// I chose not to do it.
#include <algorithm>
#include <functional>
#include <iostream>
#include <math.h>
#include <vector>
using namespace std;
void add_intersections(vector<double>& xs,
                       double x1, double y1, double r1,
                       double x2, double y2, double r2) {
    const double dx = x2 - x1;
    const double dy = y2 - y1;
    const double d = hypot(dx, dy);
    if (d <= fabs(r1 - r2) || d >= r1 + r2) return;
    const double h = (d*d - r2*r2 + r1*r1) / (2*d);
    const double k = sqrt(r1*r1 - h*h);
    const double Ax = x1 + dx * (h / d);
    xs.push_back(Ax + dy * (k / d));
    xs.push_back(Ax - dy * (k / d));
}
struct Event {
    double y;
    int idx;
    enum Kind {
        BEGIN,
        END,
        BOTTOM,
        TOP
    } kind;
};
bool operator<(const Event& e1, const Event& e2) {
    return e1.y < e2.y;
}
double integrate(double ox, double r, double x1, double x2) {
    x1 = max(-1.0, min((x1 - ox) / r, 1.0));
    x2 = max(-1.0, min((x2 - ox) / r, 1.0));
    const double hi = (asin(x2) + x2*sqrt(1-x2*x2))/2;
    const double lo = (asin(x1) + x1*sqrt(1-x1*x1))/2;
    return r*r*(hi - lo);
}
void do_testcase(int X, int Y, int N1, int N2) {
    int N = N1 + N2;
    vector<double> x(N);
    vector<double> y(N);
    vector<double> r(N);
    for (int i = 0; i < N; i++) {
        cin >> x[i] >> y[i];
        if (i < N1) r[i] = 0.58; else r[i] = 1.31;
    }
    vector<double> xs = {0, (double)X};
    for (int i = 0; i < N; i++) {
        xs.push_back(x[i] - r[i]);
        xs.push_back(x[i] + r[i]);
        for (int j = 1; j < N; j++) {
            add_intersections(xs, x[i], y[i], r[i], x[j], y[j], r[j]);
        }
    }
    xs.erase(remove_if(xs.begin(), xs.end(),
                       [&](const double x) {
                           return x < 0 || x > X;
                       }),
             xs.end());
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    double result = X*Y;
    for (int i = 1; i < xs.size(); i++) {
        const double x0 = (xs[i] + xs[i - 1])/2;
        const double width = xs[i] - xs[i - 1];
        vector<Event> events = {{0, -1, Event::BOTTOM},
                                {(double)Y, -1, Event::TOP}};
        for (int j = 0; j < N; j++) {
            const double z = r[j]*r[j] - (x0-x[j])*(x0-x[j]);
            if (z <= 0) continue;
            events.push_back(Event{y[j]-sqrt(z), j, Event::BEGIN});
            events.push_back(Event{y[j]+sqrt(z), j, Event::END});
        }
        sort(events.begin(), events.end());
        int depth = 0, last = -1;
        for (int j = 0; j < events.size(); j++) {
            if (events[j].kind == Event::BEGIN) {
                if (0 == depth++ && events[j].y > 0) last = j;
            } else if (events[j].kind == Event::END) {
                if (0 == --depth && events[j].y > 0) {
                    const int i2 = events[j].idx;
                    const double a2 = integrate(x[i2], r[i2],
                                                xs[i - 1], xs[i]);
                    if (events[last].kind == Event::BEGIN) {
                        // area bounded by two arcs
                        const int i1 = events[last].idx;
                        const double a1 = integrate(x[i1], r[i1],
                                                    xs[i - 1], xs[i]);
                        result -= a1 + a2 + width * (y[i2] - y[i1]);
                    } else {
                        // area bounded by this arc and the bottom
                        result -= a2 + width * y[i2];
                    }
                }
            } else if (events[j].kind == Event::BOTTOM) {
                if (depth > 0) last = j;
            } else {
                if (depth > 0) {
                    if (events[last].kind == Event::BOTTOM) {
                        // entire column is covered
                        result -= width * Y;
                    } else {
                        // area bounded by the top and the last arc
                        const int i1 = events[last].idx;
                        const double a1 = integrate(x[i1], r[i1],
                                                    xs[i - 1], xs[i]);
                        result -= a1 + width * (Y - y[i1]);
                    }
                }
                break;
            }
        }
    }
    printf("%.2f\n", result);
}
int main() {
    for (;;) { 
        int X, Y, N1, N2; cin >> X >> Y >> N1 >> N2;
        if (X == 0) return 0;
        do_testcase(X, Y, N1, N2);
    }
}
