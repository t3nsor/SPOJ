// 2024-09-24
// We use an angle sweep approach. We need a point inside the polygon; the
// easiest is the centroid. Consider rotating the polygon around the centroid.
// Each vertex then has a range of rotation angles in which it's the vertex with
// the greatest x-coordinate, and a similar range for least x-coordinate and
// greatest/least y-coordinate. And we can compute all rotation angles at which
// the vertex with the current greatest/least x/y-coordinate changes. In arcs
// between every consecutive pair of such angles, the function we need to
// maximize/minimize is the function
//   f(x) = r0 cos(x + a0) + r1 sin(x + a1) - r2 cos(x + a2) - r3 sin(x + a3)
// where (r0, a0) are the polar coordinates relative to the centroid of the
// current vertex with the greatest x-coordinate, and so on. The max/min occur
// either at the endpoints of the interval, or at some point within the interval
// where the derivative is zero, which we can find easily. This is an O(n log n)
// solution because we use `std::sort` on the list of angles at which a current
// vertex changes (4n such angles total) but it's not hard to do the sort in
// O(n) if you want to: because we're given the vertices in order, we
// automatically get 4 lists of angles, in sorted order, at which the vertex
// with the current greatest/least x/y-coordinate changes, and we could just
// merge those 4 sorted lists into 1.
#include <algorithm>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
const double pi = acos(-1.0);
struct Event {
    double angle;
    int id;
    int type;
    friend bool operator<(const Event& e1, const Event& e2) {
        return e1.angle < e2.angle;
    }
};
int main() {
    int N; cin >> N;
    vector<double> X(N), Y(N);
    double Ox = 0, Oy = 0;
    for (int i = 0; i < N; i++) {
        cin >> X[i] >> Y[i];
        Ox += X[i]; Oy += Y[i];
    }
    Ox /= N; Oy /= N;
    for (int i = 0; i < N; i++) {
        X[i] -= Ox; Y[i] -= Oy;
    }
    vector<Event> events;
    vector<double> arg;
    vector<double> r;
    for (int i = 0; i < N; i++) {
        double a = atan2(Y[i], X[i]);
        if (a < 0) a += 2*pi;
        arg.push_back(a);
        r.push_back(hypot(X[i], Y[i]));
    }
    double d = arg[1] - arg[0];
    if (d < 0) d += 2*pi;
    const int dir = d > pi ? -1 : 1;
    for (int i = 0; i < N; i++) {
        const int prev = (i - dir + N) % N;
        Event e;
        e.angle = pi/2 - atan2(Y[i] - Y[prev], X[i] - X[prev]);
        if (e.angle < 0) e.angle += 2*pi;
        e.id = prev;
        for (e.type = 0; e.type <= 3; e.type++) {
            events.push_back(e);
            e.angle += pi/2;
            if (e.angle >= 2*pi) e.angle -= 2*pi;
        }
    }
    sort(events.begin(), events.end());
    int cur[4];
    for (int i = 0; i < events.size(); i++) {
        cur[events[i].type] = events[i].id;
    }
    double m = 40000, M = 0;
    // no lambdas in C++03, so use a macro instead
#define OPTIMIZE(a) \
    { \
        const double angle = a; \
        double xmax = r[cur[0]] * cos(arg[cur[0]] + angle); \
        double ymax = r[cur[1]] * sin(arg[cur[1]] + angle); \
        double xmin = r[cur[2]] * cos(arg[cur[2]] + angle); \
        double ymin = r[cur[3]] * sin(arg[cur[3]] + angle); \
        if (xmax < xmin) swap(xmax, xmin); \
        if (ymax < ymin) swap(ymax, ymin); \
        m = min(m, xmax - xmin + ymax - ymin); \
        M = max(M, xmax - xmin + ymax - ymin); \
    }
    for (int i = 0; i < events.size(); i++) {
        OPTIMIZE(events[i].angle);
        const double c0 = arg[cur[0]], c1 = arg[cur[1]],
                     c2 = arg[cur[2]], c3 = arg[cur[3]];
        const double r0 = r[cur[0]], r1 = r[cur[1]],
                     r2 = r[cur[2]], r3 = r[cur[3]];
        const double p =   r0 * sin(c0) - r1 * cos(c1)
                         - r2 * sin(c2) + r3 * cos(c3);
        const double q = - r0 * cos(c0) - r1 * sin(c1)
                         + r2 * cos(c2) + r3 * sin(c3);
        OPTIMIZE(q == 0 ? pi/2 : atan(p / q));
        cur[events[i].type] = events[i].id;
    }
    printf("%.6f %.6f\n", m, M);
}
