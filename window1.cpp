// 2023-10-01
// The idea is to delete the part of the polygon that is below the line y = y1.
// In doing so, we might create new vertices with y = y1, and we might split the
// original polygon into multiple polygons. If we know how to compute the result
// of this operation, we can repeat it 3 more times in order to determine the
// intersection of the original polygon with the window (where, if there are
// multiple polygons, this operation must be applied to each one separately).
// The number of polygons left at the end is the answer.
//
// In order to delete what is below the line, we walk around the polygon
// starting from a point that is above the line. (If there are no such points,
// we delete the entire polygon and go on to the next one.) Every time we cross
// the line, we create a new vertex on the line. Then we walk around a second
// time and mark all vertices below the line for removal. (The polygon is stored
// in a linked list: each vertex remembers the next one in counterclockwise
// order.) Finally, we "fix up" vertices that are on the line: the leftmost one
// connects to the second-leftmost one, the third-leftmost one connects to the
// fourth-leftmost one, and so on.
//
// In order to simplify the implementation, we ensure that no point is ever on
// the line at the beginning. We can do this by scaling all coordinates and
// contracting the window a bit (as if (x1, y1) and (x2, y2) were replaced by
// (x1+eps, y1+eps) and (x2-eps, y2-eps)). The code below uses a scale factor of
// 4, but I guess you could also use 3.
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
struct Point {
    int x;
    int y;
    int next;
    bool visited;
    bool removed;
};
void cut(vector<Point>& points, int start, int y0) {
    // Find a point that's above the line.
    int cur = start;
    while (points[cur].y < y0 && points[cur].next != start) {
        cur = points[cur].next;
    }
    if (points[cur].y < y0) {
        // Entire polygon is below the line. Remove all points.
        cur = start;
        do {
            points[cur].removed = true;
            cur = points[cur].next;
        } while (cur != start);
        return;
    }
    start = cur;
    // `start` is now above the line.
    vector<pair<int, int>> on_line;
    do {
        int next = points[cur].next;
        if ((points[cur].y > y0 && points[next].y < y0) ||
            (points[cur].y < y0 && points[next].y > y0)) {
            // transition from above or below to vice versa
            points.emplace_back();
            points.back().x = points[cur].x;
            points.back().y = y0;
            points.back().next = next;
            on_line.emplace_back(points[cur].x, points.size() - 1);
            next = points[cur].next = points.size() - 1;
        }
        cur = next;
    } while (cur != start);
    sort(on_line.begin(), on_line.end());
    // remove anything that's below the line
    do {
        int next = points[cur].next;
        points[cur].visited = true;
        if (points[next].y < y0) {
            // we must be on the line
            points[next].removed = true;
            points[cur].next = points[next].next;
            continue;
        }
        cur = next;
    } while (cur != start);
    // fix up anything that's on the line
    for (int i = 0; i < on_line.size(); i += 2) {
        points[on_line[i].second].next = on_line[i + 1].second;
    }
}
void do_testcase() {
    int x1, y1, x2, y2;
    cin >> x1 >> y2 >> x2 >> y1;
    int n; cin >> n;
    vector<Point> points(n);
    for (int i = 0; i < n; i++) {
        cin >> points[i].x >> points[i].y;
        points[i].x *= 4;
        points[i].y *= 4;
        points[i].next = (i + 1) % n;
    }
    x1 = 4 * x1 + 1;
    x2 = 4 * x2 - 1;
    y1 = 4 * y1 + 1;
    y2 = 4 * y2 - 1;
    // Cut at y1.
    for (int i = 0; i < points.size(); i++) {
        if (points[i].removed || points[i].visited) continue;
        cut(points, i, y1);
    }
    // Rotate all points 90 degrees CCW and then cut at x1.
    for (int i = 0; i < points.size(); i++) {
        points[i].visited = false;
        points[i].x = -exchange(points[i].y, points[i].x);
    }
    for (int i = 0; i < points.size(); i++) {
        if (points[i].removed || points[i].visited) continue;
        cut(points, i, x1);
    }
    // Rotate all points 90 degrees CCW and then cut at y2. (Keep in mind that
    // the line y = y2 is mapped to y = -y2)
    for (int i = 0; i < points.size(); i++) {
        points[i].visited = false;
        points[i].x = -exchange(points[i].y, points[i].x);
    }
    for (int i = 0; i < points.size(); i++) {
        if (points[i].removed || points[i].visited) continue;
        cut(points, i, -y2);
    }
    // Rotate all points 90 degrees CCW and then cut at x2. (Again, a minus sign
    // is needed here.)
    for (int i = 0; i < points.size(); i++) {
        points[i].visited = false;
        points[i].x = -exchange(points[i].y, points[i].x);
    }
    for (int i = 0; i < points.size(); i++) {
        if (points[i].removed || points[i].visited) continue;
        cut(points, i, -x2);
    }
    // Clear the "visited" property and check how many distinct cycles we have.
    for (int i = 0; i < points.size(); i++) {
        points[i].visited = false;
    }
    int result = 0;
    for (int i = 0; i < points.size(); i++) {
        if (points[i].removed || points[i].visited) continue;
        ++result;
        int cur = i;
        while (!points[i].visited) {
            points[i].visited = true;
            i = points[i].next;
        }
    }
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
