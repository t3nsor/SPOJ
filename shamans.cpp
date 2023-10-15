// 2023-10-14
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <utility>
#include <vector>
using namespace std;
bool is_nonconvex(pair<int, int> A, pair<int, int> B, pair<int, int> C) {
    return (B.first - A.first)*(C.second - A.second) <=
	   (B.second - A.second)*(C.first - A.first);
}
using ll = long long;
double area(pair<int, int> A, pair<int, int> B, pair<int, int> C) {
    ll x1 = B.first - A.first;
    ll y1 = B.second - A.second;
    ll x2 = C.first - A.first;
    ll y2 = C.second - A.second;
    return abs(x1*y2 - x2*y1) / 2.0;
}
void do_testcase() {
    int n; cin >> n;
    vector<pair<int, int>> points(n);
    for (int i = 0; i < n; i++) {
        cin >> points[i].first >> points[i].second;
    }
    sort(points.begin(), points.end());
    vector<pair<int, int>> hull;
    for (int i = 0; i < points.size(); i++) {
        while (hull.size() >= 2 && is_nonconvex(hull[hull.size() - 2],
                                                hull.back(),
                                                points[i])) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }
    int s = hull.size();
    for (int i = points.size() - 2; i >= 0; i--) {
        while (hull.size() >= s + 1 && is_nonconvex(hull[hull.size() - 2],
                                                    hull.back(),
                                                    points[i])) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }
    hull.pop_back();
    n = hull.size();
    if (n == 3) {
        // degenerate case
        printf("%.1f\n", area(hull[0], hull[1], hull[2]));
        return;
    }
    double result = 0;
    for (int i = 0; i < n - 3; i++) {
        int j, m;
        for (int k = i + 2; k + 1 < n; k++) {
            if (k == i + 2) {
                j = i + 1;
                m = k + 1;
            } else {
                while (j + 1 < k &&
                       area(hull[i], hull[j + 1], hull[k]) >=
                       area(hull[i], hull[j], hull[k])) {
                    j++;
                }
            }
            if (m == k) ++m;
            while (m + 1 < n &&
                   area(hull[i], hull[k], hull[m + 1]) >=
                   area(hull[i], hull[k], hull[m])) {
                m++;
            }
            result = max(result, area(hull[i], hull[j], hull[k]) +
                                 area(hull[i], hull[k], hull[m]));
        }
    }
    printf("%.1f\n", result);
}
int main() {
    int t; cin >> t;
    while (t--) do_testcase();
}
