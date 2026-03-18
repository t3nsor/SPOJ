// 2026-03-18
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;
vector<double> X, Y, R;
vector<int> exist;
void get_tangents(double x1, double y1, double r1,
                  double x2, double y2, double r2,
                  double k, double (&result)[2][3]) {
    // shift circle 1 to origin
    double x = x2 - x1;
    double y = y2 - y1;

    // compute tangent lines
    const double phase = atan2(-y, x);
    const double ac0 = acos((r1 + k*r2) / hypot(x, y));
    const double ac1 = -ac0;

    const double theta0 = ac0 - phase;
    const double theta1 = ac1 - phase;

    const double A0 = cos(theta0);
    const double B0 = sin(theta0);
    const double C0 = -r1;
    const double A1 = cos(theta1);
    const double B1 = sin(theta1);
    const double C1 = -r1;

    // shift back
    result[0][0] = A0;
    result[0][1] = B0;
    result[0][2] = C0 - A0*x1 - B0*y1;
    result[1][0] = A1;
    result[1][1] = B1;
    result[1][2] = C1 - A1*x1 - B1*y1;
}
int calc() {
    vector<int> circles;
    for (int i = 0; i < exist.size(); i++) {
        if (exist[i]) circles.push_back(i);
    }
    // special case (easy): all circles are nested
    sort(circles.begin(), circles.end(),
         [&](int i, int j) { return R[i] < R[j]; });
    bool nested = true;
    for (int i = 1; i < circles.size(); i++) {
        if (R[circles[i]] - R[circles[i - 1]] <
            hypot(X[circles[i]] - X[circles[i - 1]],
                  Y[circles[i]] - Y[circles[i - 1]])) {
            nested = false;
        }
    }
    if (nested) return circles.size();
    // otherwise, an optimal line can always be moved until it's tangent to two
    // circles
    int best = 0;
    for (int i = 1; i < circles.size(); i++) {
        for (int j = 0; j < i; j++) {
            const double dist = hypot(X[circles[i]] - X[circles[j]],
                                      Y[circles[i]] - Y[circles[j]]);
            if (fabs(R[circles[i]] - R[circles[j]]) >= dist) {
                continue;  // nested
            }
            const auto get_count = [&](double (&line)[3]) {
                int result = 0;
                for (int k = 0; k < circles.size(); k++) {
                    const double x = X[circles[k]];
                    const double y = Y[circles[k]];
                    const double r = R[circles[k]];
                    if (fabs(line[0]*x + line[1]*y + line[2]) <
                        r + 1e-6) ++result;
                }
                return result;
            };
            // find the 2 external tangents (they always exist)
            double line[2][3];
            get_tangents(X[circles[i]], Y[circles[i]], R[circles[i]],
                         X[circles[j]], Y[circles[j]], R[circles[j]],
                         +1, line);
            best = max(best, get_count(line[0]));
            best = max(best, get_count(line[1]));
            if (dist > R[circles[i]] + R[circles[j]]) {
                // find the 2 internal tangents
                get_tangents(X[circles[i]], Y[circles[i]], R[circles[i]],
                             X[circles[j]], Y[circles[j]], R[circles[j]],
                             -1, line);
                best = max(best, get_count(line[0]));
                best = max(best, get_count(line[1]));
            }
        }
    }
    return best;
}
int main() {
    int M; cin >> M;
    while (M--) {
        int type; cin >> type;
        if (type == 1) {
            double x, y, r; cin >> x >> y >> r;
            X.push_back(x); Y.push_back(y); R.push_back(r); exist.push_back(1);
        } else if (type == 2) {
            int i; cin >> i; exist[i - 1] = 0;
        } else {
            cout << calc() << '\n';
        }
    }
}
