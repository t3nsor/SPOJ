// 2026-04-27
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
double integrate(double x1, double x2) {
    return 0.5*(asin(x2) + x2*sqrt(1-x2*x2) - asin(x1) - x1*sqrt(1-x1*x1));
}
double calc(double x1, double y1, double x2, double y2) {
    if (x1 > 1 || x2 < -1 || y1 > 1 || y2 < 0) return 0.0;
    vector<double> events = {x1, x2};
    double sq1 = sqrt(1-y1*y1);
    if (sq1 > x1 && sq1 < x2) events.push_back(sq1);
    if (-sq1 > x1 && -sq1 < x2) events.push_back(-sq1);
    double sq2 = sqrt(1-y2*y2);
    if (sq2 > x1 && sq2 < x2) events.push_back(sq2);
    if (-sq2 > x1 && -sq2 < x2) events.push_back(-sq2);
    sort(events.begin(), events.end());
    double result = 0;
    for (int i = 1; i < events.size(); i++) {
        double xm = (events[i] + events[i - 1]) / 2.0;
        double ym = sqrt(1-xm*xm);
        if (ym < y1) continue;
        if (ym < y2) {
            result += integrate(events[i - 1], events[i])
                      - y1*(events[i] - events[i - 1]);
        } else {
            result += (y2 - y1)*(events[i] - events[i - 1]);
        }
    }
    return result;
}
int main() {
    int x, y, r, x1, y1, x2, y2;
    while (scanf("%d%d%d%d%d%d%d", &x, &y, &r, &x1, &y1, &x2, &y2) == 7) {
        if (r == 0) { puts("0"); continue; }
        double X1 = double(x1 - x) / r;
        double Y1 = double(y1 - y) / r;
        double X2 = double(x2 - x) / r;
        double Y2 = double(y2 - y) / r;
        if (X1 > X2) swap(X1, X2);
        if (Y1 > Y2) swap(Y1, Y2);
        printf("%.6f\n", r*r*(calc(max(X1, -1.0), max(Y1, 0.0),
                                   min(X2, 1.0), min(Y2, 1.0)) +
                              calc(max(X1, -1.0), max(-Y2, 0.0),
                                   min(X2, 1.0), min(-Y1, 1.0))));
    }
}
