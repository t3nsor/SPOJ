// 2025-05-07
// This is a simulation problem. The derivative of the total area covered by the
// automatic umbrellas is a piecewise constant function, and changes only when
// an automatic umbrella touches the edge of the crosswalk or an edge of an
// automatic umbrella coincides with an edge of another automatic umbrella. So
// all we have to do is find all times at which the derivative changes. Between
// any two consecutive times, the area swept out by the function whose value is
// the total area covered by the automatic umbrellas is the area of a trapezoid.
// Special cases include:
// * If an automatic umbrella has a velocity of zero or covers the entire
//   sidewalk, we must be careful not to get into an infinite loop simulating it
// * If two automatic umbrellas have the same position and velocity (either in a
//   given interval of time or for all time) we can treat them as if they never
//   intersect, because they in fact cannot contribute to the derivative
//   changing.
#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>
using namespace std;
// `Interval` describes an interval of time during which an automatic umbrella
// moves with constant velocity.
struct Interval {
    double st;   // time at beginning of interval
    double et;   // time at end of interval
    double pos;  // initial position of center
    double v;    // velocity during this interval
};

double findIntersection(double t1, double t2,
                        double x1, double x2,
                        double v1, double v2) {
    if (v1 == v2) return -1.0;
    return (x2 - x1 + v1*t1 - v2*t2) / (v1 - v2);
}

void addIntersections(vector<double>& events,
                      const Interval& i1, double l1,
                      const Interval& i2, double l2) {
    for (int p = 0; p < 2; p++) {
        const double x1 = (p == 0) ? i1.pos - l1 / 2 : i1.pos + l1 / 2;
        for (int q = 0; q < 2; q++) {
            const double x2 = (q == 0) ? i2.pos - l2 / 2 : i2.pos + l2 / 2;
            const double event = findIntersection(i1.st, i2.st,
                                                  x1, x2,
                                                  i1.v, i2.v);
            if (event >= i1.st && event <= i1.et &&
                event >= i2.st && event <= i2.et) events.push_back(event);
        }
    }
}
void do_testcase() {
    int N, W, T, vol; cin >> N >> W >> T >> vol;
    vector<double> X(N), L(N), V(N);
    vector<vector<Interval>> intervals(N);
    vector<double> events = {0.0, double(T)};
    for (int i = 0; i < N; i++) {
        cin >> X[i] >> L[i] >> V[i];
        X[i] += L[i] / 2;
        if (L[i] == W) V[i] = 0;
        if (V[i] == 0) {
            intervals[i].push_back({0, double(T), X[i], 0.0});
            continue;
        }
        double curT;
        if (V[i] > 0) {
            curT = ((W - L[i] / 2) - X[i]) / V[i];
        } else {
            curT = (L[i] / 2 - X[i]) / V[i];
        }
        if (curT > T) curT = T;
        intervals[i].push_back({0, curT, X[i], V[i]});
        int factor = -1;
        while (curT < T) {
            events.push_back(curT);
            double nextT = curT + (W - L[i]) / fabs(V[i]);
            if (nextT > T) nextT = T;
            if (factor * V[i] > 0) {
                intervals[i].push_back({curT, nextT, L[i] / 2,
                                        factor * V[i]});
            } else {
                intervals[i].push_back({curT, nextT, W - L[i] / 2,
                                        factor * V[i]});
            }
            factor *= -1;
            curT = nextT;
        }
        for (int j = 0; j < i; j++) {
            int k2 = 0;
            for (int k1 = 0; k1 < intervals[i].size(); k1++) {
                while (k2 < intervals[j].size()) {
                    if (intervals[j][k2].et < intervals[i][k1].st) {
                        ++k2;
                        continue;
                    }
                    if (intervals[j][k2].st > intervals[i][k1].et) break;
                    addIntersections(events,
                                     intervals[i][k1], L[i],
                                     intervals[j][k2], L[j]);
                    ++k2;
                }
                if (k2 > 0) --k2;
            }
        }
    }
    sort(events.begin(), events.end());
    double result = 0;
    double lastl;
    vector<int> I(N, 0);
    for (int i = 0; i < events.size(); i++) {
        const double t = events[i];
        vector<pair<double, double>> segments;
        for (int j = 0; j < N; j++) {
            while (I[j] + 1 < intervals[j].size() &&
                   intervals[j][I[j]].et < t) {
                ++I[j];
            }
            const auto& interval = intervals[j][I[j]];
            const double x = interval.pos + interval.v * (t - interval.st);
            segments.emplace_back(x - L[j] / 2, x + L[j] / 2);
        }
        double l = 0;
        sort(segments.begin(), segments.end());
        double lastx = 0;
        for (const auto& segment : segments) {
            if (segment.first > lastx) {
                l += segment.first - lastx;
            }
            lastx = max(lastx, segment.second);
        }
        if (W > lastx) l += W - lastx;
        if (i > 0) {
            result += (l + lastl) * (t - events[i - 1])/ 2;
        }
        lastl = l;
    }
    printf("%.2f\n", result * vol);
}
int main() {
    int T; cin >> T; while (T--) do_testcase();
}
