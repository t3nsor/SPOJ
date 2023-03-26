// 2023-03-19
// The idea is that if we ignore all horizontal edges (direction of 3 or 6) when
// building the polygon then we get another valid polygon that contains only B
// rhombs, and in the same number as the original polygon; the associated A and
// C rhombs, which have horizontal edges, don't appear in this new polygon.
// Similarly, ignoring edges in directions 1 and 4 gives only A rhombs, and
// ignoring edges in directions 2 and 5 gives only C rhombs. To get the number
// of rhombs in one of these reduced polygons, all we have to do is compute the
// area of the polygon. To simplify the code, we define a `Coord`
// type which represents an element of the ring Z[sqrt(3)], and we take each
// rhomb to have a side length of 2, so that all coordinates fall within that
// ring. (That means the area of a single rhomb is 2 sqrt(3).)
#include <iostream>
#include <vector>
using namespace std;
struct Coord {
    long long a;
    long long b;
    // represents a + b sqrt(3)
    Coord& operator+=(Coord other) {
        a += other.a;
        b += other.b;
        return *this;
    }
};
Coord operator-(Coord c1, Coord c2) {
    return {c1.a - c2.a, c1.b - c2.b};
}
Coord operator*(Coord c1, Coord c2) {
    return {c1.a * c2.a + 3 * c1.b * c2.b,
            c1.a * c2.b + c2.a * c1.b};
}
struct Point {
    Coord x;
    Coord y;
    Point& operator+=(Point other) {
        x += other.x;
        y += other.y;
        return *this;
    }
};
constexpr Point dir[6] = {
    {{-1, 0}, {0,  1}},  // 1
    {{ 1, 0}, {0,  1}},  // 2
    {{ 2, 0}, {0,  0}},  // 3
    {{ 1, 0}, {0, -1}},  // 4
    {{-1, 0}, {0, -1}},  // 5
    {{-2, 0}, {0,  0}}   // 6
};
Point operator*(int k, Point p) {
    return {{k * p.x.a, k * p.x.b}, {k * p.y.a, k * p.y.b}};
}
Coord twice_area(const vector<Point>& vec) {
    Coord result = {0, 0};
    for (int i = 0; i < vec.size(); i++) {
        int next = i + 1; if (next == vec.size()) next = 0;
        result += vec[next].x * vec[i].y - vec[i].x * vec[next].y;
    }
    return {result.a, result.b};
}
void do_testcase() {
    int n; cin >> n;
    Point pos[3] = {};
    vector<Point> v0 = {pos[0]};  // excludes 1, 4
    vector<Point> v1 = {pos[1]};  // excludes 2, 5
    vector<Point> v2 = {pos[2]};  // excludes 3, 6
    while (n--) {
        int d, k;
        cin >> d >> k;
        if (k == 0) continue;  // probably does not happen
        --d;
        Point vec = k * dir[d];
        if (d != 0 && d != 3) {
            pos[0] += vec;
            v0.push_back(pos[0]);
        }
        if (d != 1 && d != 4) {
            pos[1] += vec;
            v1.push_back(pos[1]);
        }
        if (d != 2 && d != 5) {
            pos[2] += vec;
            v2.push_back(pos[2]);
        }
    }
    Coord a0 = twice_area(v0);
    Coord a1 = twice_area(v1);
    Coord a2 = twice_area(v2);
    cout << a0.b / 4 << ' ' << a2.b / 4 << ' ' << a1.b / 4 << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
