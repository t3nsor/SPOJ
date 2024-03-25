// 2024-03-25
// Use 0.00005 degree as the unit of longitude, and 0.0001/(2x) as the unit of
// time, where v is the speed of the fire in degrees per hour. Then, in the new
// units,
// * the speed of the fire is 1
// * all longitudes are scaled up by 20000
// * all times at which fires start are scaled up by 20000v (making them even
//   even integers)
// The last location to catch fire will be at an integer, and this will happen
// at an integer-valued time which we can find using binary search. In our
// binary search loop we use a simple sweep line algorithm. If there are any
// uncovered intervals of positive length, our time is too early. When the time
// is just right, there should be no uncovered intervals of positive length but
// there should be a point such that if all endings were ordered before all
// beginnings, there would be an uncovered interval of zero length. Such a point
// is a potential last point. If there are no potential last points, the time is
// too early.
//
// Problem statement on SPOJ doesn't specify bounds. The official IPSC test
// data have v <= 20 and t <= 134.6.
#include <algorithm>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
using namespace std;
int get_and_scale100() {
    double z; scanf("%lf", &z); return lround(z * 100);
}
constexpr int L = 7200000;  // size of the world
int dist(int x, int y) {
    const int d = abs(x - y);
    return min(d, L - d);
}
void do_testcase(int v) {
    int N; scanf("%d", &N);
    vector<int> t(N), x(N);
    int tmin = 1e9;
    for (int i = 0; i < N; i++) {
        const int y = get_and_scale100();
        if (y > 13460) throw;
        t[i] = 2*v*y;
        x[i] = 200*get_and_scale100();
        tmin = min(tmin, t[i]);
    }
    int l = tmin;
    int r = tmin + L / 2;
    vector<int> x2(N);
    for (;;) {
        const int m = (l + r) / 2;
        // rotate the points so that the zero point is not at an endpoint
        vector<char> possible_origin(10001, 1);
        for (int i = 0; i < N; i++) {
            if (m < t[i]) continue;
            const int size = m - t[i];
            const int p1 = ((x[i] - size) % L + L) % L;
            const int p2 = ((x[i] + size) % L + L) % L;
            if (p1 <= 10000) possible_origin[p1] = 0;
            if (p2 <= 10000) possible_origin[p2] = 0;
        }
        const int origin = find(possible_origin.begin(),
                                possible_origin.end(),
                                1) - possible_origin.begin();
        for (int i = 0; i < N; i++) {
            x2[i] = ((x[i] - origin) % L + L) % L;
        }
        vector<pair<int, int>> events;  // -1 = end, 1 = start
        int depth = 0;  // initially, the # of times 0 is covered
        for (int i = 0; i < N; i++) {
            if (m < t[i]) continue;
            const int size = m - t[i];
            if (size > dist(x2[i], 0)) {
                ++depth;
            }
            if (size == 0) {
                // ignore this interval since it doesn't contribute to whether
                // we have an uncovered interval of positive length, nor to
                // whether a point can be a possible last point
                continue;
            }
            const int begin = ((x2[i] - size) % L + L) % L;
            const int end = ((x2[i] + size) % L + L) % L;
            events.emplace_back(begin, 1);
            events.emplace_back(end, -1);
        }
        sort(events.begin(), events.end());
        int last_uncovered = 0;
        int best_last_point = -1;
        for (const auto& event : events) {
            if (depth == 0) {
                // this must be a start event
                if (event.first > last_uncovered) {
                    // we have an uncovered interval of positive length
                    l = m + 1;
                    goto end;
                } else if (event.first == last_uncovered) {
                    // we have a possible last point, i.e., this point would be
                    // uncovered at any previous time
                    const int absolute = (event.first + origin) % L;
                    if (best_last_point == -1 ||
                        absolute < best_last_point) {
                        best_last_point = absolute;
                    }
                }
            }
            depth += event.second;
            if (depth == 0) {
                last_uncovered = event.first;
            }
        }
        if (depth == 0) {
            // the interval from here to the end is uncovered
            l = m + 1;
        } else if (best_last_point == -1) {
            // there are no potential last points; every point is in the interior
            // of an interval
            r = m - 1;
        } else {
            printf("%.3f ", m / (200.0 * v));
            if (best_last_point > L/2) {
                best_last_point -= L;
            }
            printf("%.3f\n", best_last_point / 20000.0);
            return;
        }
end:
        ;
    }
}
int main() {
    for (;;) {
        const int v = get_and_scale100();
        if (v > 2000) throw;
        if (v < 0) return 0;
        do_testcase(v);
    }
}
