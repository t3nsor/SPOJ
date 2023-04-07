// 2023-04-07
#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
using namespace std;
int cross(pair<int, int> a, pair<int, int> b, pair<int, int> c) {
    return (a.first - b.first) * (c.second - b.second) -
           (a.second - b.second) * (c.first - b.first);
}
int gcd(int x, int y) {
    if (x > y) return gcd(y, x);
    if (x == 0) return y;
    return gcd(y % x, x);
}
pair<int, int> normalize(pair<int, int> p) {
    if (p.second < 0) {
        p.first *= -1;
        p.second *= -1;
    } else if (p.second == 0 && p.first < 0) {
        p.first *= -1;
    }
    const int g = gcd(abs(p.first), p.second);
    return {p.first / g, p.second / g};
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
        while (hull.size() >= 2 && cross(hull[hull.size() - 2],
                                         hull[hull.size() - 1],
                                         points[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }
    int upper_size = hull.size();
    for (int i = points.size() - 2; i >= 0; i--) {
        while (hull.size() > upper_size && cross(hull[hull.size() - 2],
                                                 hull[hull.size() - 1],
                                                 points[i]) <= 0) {
            hull.pop_back();
        }
        if (i > 0) hull.push_back(points[i]);
    }
    // This should really be unordered_map, but I'm too lazy to write a hash
    // function
    map<pair<int, int>, int> M;
    for (int i = 0; i < hull.size(); i++) {
        int next = i + 1; if (next == hull.size()) next = 0;
        const pair<int, int> vec = {hull[i].first - hull[next].first,
                                    hull[i].second - hull[next].second};
        ++M[normalize(vec)];
    }
    int result = 0;
    for (int i = 0; i < hull.size(); i++) {
        int next = i + 1; if (next == hull.size()) next = 0;
        const pair<int, int> vec = {hull[i].first - hull[next].first,
                                    hull[i].second - hull[next].second};
        const pair<int, int> pvec = {-vec.second, vec.first};
        if (M[normalize(vec)] >= 2 && M[normalize(pvec)] >= 2) ++result;
    }
    cout << result / 4 << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
