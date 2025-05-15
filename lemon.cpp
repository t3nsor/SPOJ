// 2025-05-14
// The vibes of the solution are easy to describe, but the implementation is a
// massive headache. Writing a "slow" solution is only a little bit easier, so
// finding problematic test cases is non-trivial, making it very difficult to
// find all the bugs. I probably wouldn't have been able to do it if I hadn't
// had access to the test data.
//
// The trunk casts a shadow whose length is tan(alpha) times the height of the
// trunk. We'll consider this shadow to lie along the positive x-axis, with the
// point on the trunk at height h mapped to the point with x-coordinate
// h*cot(alpha). The circles with radii r_1, ..., r_n, 0 are mapped to shadow
// circles whose centres lie on the x-axis with coordinate h_0 cot(alpha), ...,
// (h_0 + ... + h_n}) cot(alpha) having radii unchanged (r_1, ..., r_n, 0),
// respectively. Between each consecutive pair of such circles we have a
// continuum of circles obtained by interpolating linearly between the two. We
// have to find the area of the union of all these circles.
//
// The area swept out by the linear interpolations between two circles, in the
// case of neither circle containing the other, is bounded by the two end
// circles and their two outer tangent lines (see diagram on DMOJ). Considering
// just the upper half of the xy plane, the total area of the figure is the
// area under a function that is defined piecewise with each piece either a
// circular arc (whose centre is on the x-axis) or a line segment, depending on
// which circular arc or line segment has the greatest y-coordinate over that
// piece.  All we have to do is find the boundaries of all such pieces and the
// dominant segment/arc on each piece, and integrate to get the total area.
//
// There are up to 501 circles and 500 tangent lines. To obtain a sufficiently
// fast solution, some basic observations are necessary. First, if we sort all
// the circles by x-coordinate, we can use a stack-based algorithm to find out
// which circle dominates over a given interval: when processing each circle C,
// check whether the x-coordinate at which C dominates the circle at the top of
// the stack C' is less than the x-coordinate at which C' began to be dominant
// over its precedecessors. If so, then C' can be popped off the stack and the
// check can be repeated. Finding the dominant tangent in each interval is more
// complex. Though it's not obvious, it is possible for one tangent segment to
// dominate in a non-contiguous subset of the real line: it can be dominated by
// some other segment for a portion of its interior. This means we can't use the
// most obvious stack-based approach and must instead compute the upper envelope
// using a general divide-and-conquer approach, whose complexity is given in the
// literature as O(n alpha(n)) (alpha being the inverse Ackermann function).
// Once we've found the intervals on which arcs dominate relative to other arcs
// and segments dominate relative to other segments, we can do one final sweep
// over all endpoints of such intervals so that at each stage we have at most
// one arc and at most one interval and we can check whether they intersect (in
// which case the one that dominates can switch at most the number of times they
// intersect).
//
// This is probably not the simplest possible solution :)
#include <algorithm>
#include <iostream>
#include <math.h>
#include <limits>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
const double INF = numeric_limits<double>::infinity();
struct Seg {
    double m;
    double b;
    double x1;
    double x2;
};
double integrate(const Seg& s, double x1, double x2) {
    x1 = max(x1, s.x1);
    x2 = min(x2, s.x2);
    if (x1 >= x2) return 0;
    return s.b * (x2 - x1) + s.m * (x2*x2 - x1*x1) / 2;
}
double integrate(double o, double r, double x1, double x2) {
    x1 = max(x1 - o, -r);
    x2 = min(x2 - o, r);
    if (x1 >= x2 || r < 1e-10) return 0;
    return (r*r/2) * (asin(x2 / r)
                      - asin(x1 / r)
                      + (x2 / r) * sqrt(1 - (x2 / r) * (x2 / r))
                      - (x1 / r) * sqrt(1 - (x1 / r) * (x1 / r)));
}
struct Event {
    enum Curve {
        ARC,
        SEG
    } curve;
    enum Kind {
        BEGIN,
        END
    } kind;
    int id;
    double x;

    friend bool operator<(const Event& e1, const Event& e2) {
         return e1.x < e2.x ||
                (e1.x == e2.x && e1.kind == Event::END);
    }
};
void append(vector<Event>& events, int id, double lastx, double curx) {
    if (events.empty() || events.back().id != id) {
        events.push_back({Event::SEG, Event::BEGIN, id, lastx});
        events.push_back({Event::SEG, Event::END, id, curx});
    } else {
        events.back().x = curx;
    }
}
vector<Event> merge(const vector<Seg>& segs,
                    vector<Event>& E1,
                    vector<Event>& E2) {
    vector<Event> sorted(E1.size() + E2.size());
    std::merge(E1.begin(), E1.end(), E2.begin(), E2.end(), sorted.begin());
    vector<Event> result;
    int i = -1, j = -1;
    double lastx = -INF;
    for (const auto& e : sorted) {
        if (i == -1 && j >= 0 || i >= 0 && j == -1) {
            // no competition
            append(result, max(i, j), lastx, e.x);
        } else if (i >= 0 && j >= 0) {
            if (fabs(segs[i].m - segs[j].m) < 1e-10) {
                const int id = (segs[i].b >= segs[j].b ? i : j);
                append(result, id, lastx, e.x);
            } else {
                vector<double> endpoints = {lastx};
                // find intersection
                const double x = (segs[i].b - segs[j].b)/
                                 (segs[j].m - segs[i].m);
                if (x > lastx && x < e.x) endpoints.push_back(x);
                endpoints.push_back(e.x);
                for (int k = 0; k + 1 < endpoints.size(); k++) {
                    const double mid = (endpoints[k] + endpoints[k + 1]) / 2;
                    const int id =
                      (segs[i].m*mid + segs[i].b >= segs[j].m*mid + segs[j].b)
                      ? i
                      : j;
                    append(result, id, endpoints[k], endpoints[k + 1]);
                }
            }
        }
        if (e.kind == Event::END) {
            if (i == e.id) i = -1; else j = -1;
        } else {
            if (i < 0) i = e.id; else j = e.id;
        }
        lastx = e.x;
    }
    return result;
}
void do_testcase() {
    int N; cin >> N;
    double alpha; cin >> alpha;
    vector<double> h(N + 1), r(N + 1);
    for (int i = 0; i <= N; i++) {
        cin >> h[i];
        h[i] /= tan(alpha);
        if (i > 0) h[i] += h[i - 1];
    }
    for (int i = 0; i < N; i++) cin >> r[i];
    // compute intervals for circles
    vector<pair<int, double>> I;
    for (int i = 0; i < N; i++) {
        for (; !I.empty();) {
            if (h[i] + r[i] <= h[I.back().first] + r[I.back().first]) {
                // this circle is completely contained within a preceding one
                break;
            }
            const double rprev = r[I.back().first];
            const double d = h[i] - h[I.back().first];
            const double cs = (r[i]*r[i]+d*d-rprev*rprev)/(2*d*r[i]);
            const double x = h[i] - r[i]*cs;
            if (x <= I.back().second) {
                // includes case where current circle completely contains
                // previous circle
                I.pop_back();
            } else {
                // includes case where current circle doesn't intersect previous
                // circle
                I.emplace_back(i, max(x, h[i] - r[i]));
                break;
            }
        }
        if (I.empty()) I.emplace_back(i, h[i] - r[i]);
    }
    // compute events for circles
    vector<Event> arcevents;
    for (int i = 0; i < I.size(); i++) {
        arcevents.push_back({Event::ARC, Event::BEGIN,
                             I[i].first, I[i].second});
        const double end = min(h[I[i].first] + r[I[i].first],
                               (i + 1 == I.size() ? INF : I[i + 1].second));
        arcevents.push_back({Event::ARC, Event::END, I[i].first, end});
    }
    // compute line segments
    vector<Seg> segs;
    for (int i = 0; i < N; i++) {
        if (h[i + 1] + r[i + 1] <= h[i] + r[i] ||
            h[i + 1] - r[i + 1] <= h[i] - r[i]) continue;  // no tangent
        const double cs = (r[i] - r[i + 1]) / (h[i + 1] - h[i]);
        const double sn = sqrt(1 - cs*cs);
        segs.push_back({-cs/sn, (r[i] + cs*h[i])/sn,
                         h[i] + r[i]*cs,
                         h[i + 1] + r[i + 1]*cs});
    }
    vector<vector<Event>> segevents;
    for (int i = 0; i < segs.size(); i++) {
        vector<Event> v;
        v.push_back({Event::SEG, Event::BEGIN, i, segs[i].x1});
        v.push_back({Event::SEG, Event::END, i, segs[i].x2});
        segevents.push_back(move(v));
    }
    // run divide-and-conquer algorithm
    while (segevents.size() > 1) {
        vector<vector<Event>> newsegevents;
        for (int i = 0; i + 1 < segevents.size(); i += 2) {
            newsegevents.push_back(merge(segs, segevents[i], segevents[i + 1]));
        }
        if (segevents.size() % 2) newsegevents.push_back(segevents.back());
        segevents = move(newsegevents);
    }
    // add segments into main events list
    if (segevents.empty()) segevents.emplace_back();
    // final line sweep
    vector<Event> events(arcevents.size() + segevents[0].size());
    merge(arcevents.begin(), arcevents.end(),
          segevents[0].begin(), segevents[0].end(),
          events.begin());
    int i = -1, j = -1;
    double lastx = -INF;
    double result = 0;
    for (const auto& event : events) {
        if (i >= 0 && j < 0) {
            result += integrate(h[i], r[i], lastx, event.x);
        } else if (i < 0 && j >= 0) {
            result += integrate(segs[j], lastx, event.x);
        } else if (i >= 0 && j >= 0) {
            // both exist here, so we may have internal switch-over points
            vector<double> endpoints = {lastx, event.x};
            const double m = segs[j].m;
            const double b = segs[j].b;
            const double H = h[i];
            const double R = r[i];
            const double A = 1 + m*m;
            const double B = 2*m*(m*H + b);
            const double C = (m*H + b)*(m*H + b) - R*R;
            if (B*B >= 4*A*C) {
                const double x1 = H + (-B - sqrt(B*B - 4*A*C))/(2*A);
                const double x2 = H + (-B + sqrt(B*B - 4*A*C))/(2*A);
                if (x1 > lastx && x1 < event.x) endpoints.push_back(x1);
                if (x2 > lastx && x2 < event.x) endpoints.push_back(x2);
            }
            sort(endpoints.begin(), endpoints.end());
            for (int k = 0; k + 1 < endpoints.size(); k++) {
                const double mid = (endpoints[k] + endpoints[k + 1]) / 2;
                if (m*mid + b > sqrt(R*R - (mid - H)*(mid - H))) {
                    result += integrate(segs[j],
                                        endpoints[k],
                                        endpoints[k + 1]);
                } else {
                    result += integrate(H, R, endpoints[k], endpoints[k + 1]);
                }
            }
        }
        if (event.kind == Event::END) {
            if (event.curve == Event::ARC) i = -1;
            else j = -1;
        } else {
            if (event.curve == Event::ARC) i = event.id;
            else j = event.id;
        }
        lastx = event.x;
    }
    printf("%.2f\n", 2*result);
}
int main() {
    int T; cin >> T; while (T--) do_testcase();
}
