// 2024-10-07
// O(n^3) sweep plane approach. The outer loop sweeps over the z-coordinate, and
// the inner loop over the x-coordinate for boxes that intersect the current
// plane. The y-coordinate is compressed, so the inner loop takes O(n^2). The
// inner loop can be optimized to O(n log n) using a segment tree (Bentley's
// algorithm) but that isn't necessary for this problem.
#include <algorithm>
#include <set>
#include <iostream>
#include <vector>
using namespace std;
struct Event {
    int id;
    int coord;
    enum { END, BEGIN } type;
    friend bool operator<(const Event& e1, const Event& e2) {
        return e1.coord < e2.coord;
    }
};
void do_testcase() {
    int n; cin >> n;
    vector<int> x1(n), x2(n), y1(n), y2(n), z1(n), z2(n);
    set<int> ys;
    for (int i = 0; i < n; i++) {
        int ox, oy, oz, r; cin >> ox >> oy >> oz >> r;
        x1[i] = ox - r;
        x2[i] = ox + r;
        y1[i] = oy - r;
        y2[i] = oy + r;
        z1[i] = oz - r;
        z2[i] = oz + r;
        ys.insert(y1[i]);
        ys.insert(y2[i]);
    }
    vector<int> Y(ys.begin(), ys.end());
    for (int i = 0; i < n; i++) {
        y1[i] = lower_bound(Y.begin(), Y.end(), y1[i]) - Y.begin();
        y2[i] = lower_bound(Y.begin(), Y.end(), y2[i]) - Y.begin();
    }
    vector<Event> zevts;
    for (int i = 0; i < n; i++) {
        zevts.push_back(Event{i, z1[i], Event::BEGIN});
        zevts.push_back(Event{i, z2[i], Event::END});
    }
    sort(zevts.begin(), zevts.end());
    int lastz = -10000;
    long long result = 0;
    vector<Event> xevts;
    for (const auto& zevt : zevts) {
        if (zevt.coord > lastz) {
            sort(xevts.begin(), xevts.end());
            vector<int> depth(Y.size(), 0);
            int lcov = 0;
            int lastx = -10000;
            int result1d = 0;
            for (const auto& xevt : xevts) {
                result1d += (xevt.coord - lastx) * lcov;
                lastx = xevt.coord;
                if (xevt.type == Event::BEGIN) {
                    for (int i = y1[xevt.id]; i < y2[xevt.id]; i++) {
                        if (1 == ++depth[i]) {
                            lcov += Y[i + 1] - Y[i];
                        }
                    }
                } else {
                    for (int i = y1[xevt.id]; i < y2[xevt.id]; i++) {
                        if (0 == --depth[i]) {
                            lcov -= Y[i + 1] - Y[i];
                        }
                    }
                }
            }
            result += (zevt.coord - lastz) * (long long)result1d;
            lastz = zevt.coord;
        }
        const int id = zevt.id;
        if (zevt.type == Event::BEGIN) {
            xevts.push_back(Event{id, x1[id], Event::BEGIN});
            xevts.push_back(Event{id, x2[id], Event::END});
        } else {
            xevts.erase(remove_if(xevts.begin(), xevts.end(),
                                  [&](const auto& xevt) {
                                      return xevt.id == id;
                                  }),
                        xevts.end());
        }
    }
    cout << result << '\n';
}
int main() {
    int m; cin >> m;
    while (m--) do_testcase();
}
