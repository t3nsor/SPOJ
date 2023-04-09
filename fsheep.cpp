// 2023-04-08
// This problem is notoriously tricky, but it turns out that there are no actual
// pathological cases in the test data (at least the version that's on SPOJ).
// By "pathological case" I mean (i) two consecutive sides of the fence having
// nontrivial intersection, or (ii) the farmer not being in the interior of the
// pen. There are, of course, cases where a side of the fence can be parallel to
// a ray from the origin (the sample input shows such a case). The main
// techniques used to get AC here are:
// (1) Avoid floating point; use only integer math. In particular, do not
//     convert vectors (x, y) into their arguments (`atan2`), but instead leave
//     them as-is and compare them using determinants and case analysis.
// (2) Rotate the fence so that the post with the smallest angle is at the
//     beginning. However, if there are two posts with the smallest angle, we
//     need to make sure they're both at the beginning.
// (3) Consider separately the cases of (i) sheep located at origin, (ii) sheep
//     lies on a ray from the origin to some fence post (possibly two), and
//     (iii) none of the above (i.e. angle strictly between two post angles).

#include <algorithm>
#include <cstdio>
#include <utility>
#include <vector>
using namespace std;
using ll = long long;
struct angle {
    // |x|, |y| <= 32000
    int x;
    int y;
    friend bool operator<(angle a1, angle a2) {
        const int cross = a1.x * a2.y - a2.x * a1.y;
        if (cross > 0) {
            // counterclockwise turn from a1 to a2
            // a2 is bigger, except when a1 is strictly in the bottom half
            // plane and a2 is not
            return !(a1.y < 0 && a2.y >= 0);
        } else if (cross < 0) {
            // reverse of above
            return a1.y >= 0 && a2.y < 0;
        } else {
            const int dot = a1.x * a2.x + a1.y * a2.y;
            if (dot > 0) return false;  // a1 == a2
            // a1, a2 are antiparallel; whichever one has angle in the range
            // [0, pi) is smaller
            return a1.y > 0 || (a1.y == 0 && a1.x > 0);
        }
    }
    friend bool operator==(angle a1, angle a2) {
        return !(a1 < a2) && !(a2 < a1);
    }
};
int do_testcase() {
    int V, S; scanf("%d %d", &V, &S);
    vector<angle> angles;
    for (int i = 0; i < V; i++) {
        angle a; scanf("%d %d", &a.x, &a.y);
        angles.push_back(a);
    }

    auto me = min_element(angles.begin(), angles.end());
    if (me == angles.begin() && angles.back() == angles.front()) {
        me = angles.end() - 1;
    }
    rotate(angles.begin(), me, angles.end());

    int result = 0;
    for (int i = 0; i < S; i++) {
        int x, y; scanf("%d %d", &x, &y);
        if (x == 0 && y == 0) {
            result++;
            continue;
        }
        angle a = {x, y};
        // determine which slice this sheep falls into
        auto er = equal_range(angles.begin(), angles.end(), a);
        if (er.second != er.first) {
            // The ray from the origin to the sheep's position is incident upon
            // one or two of the fence points. To determine whether the sheep
            // is inside or on the fence, we compare it with the fence point
            // that is most distant from the origin.
            --er.second;
            const int norm1 = er.first->x * er.first->x +
                              er.first->y * er.first->y;
            const int norm2 = er.second->x * er.second->x +
                              er.second->y * er.second->y;
            // (in case of only one point, `norm1` and `norm2` are the same)
            if (x * x + y * y <= max(norm1, norm2)) ++result;
            continue;
        }
        // angle could be greater than all fence post angles, meaning that the
        // sheep falls into the slice between the last and first posts
        const auto next_it = (er.first == angles.end()
                              ? angles.begin()
                              : er.first);
        const auto next = *next_it;
        const auto prev = (next_it == angles.begin()
                           ? angles.back()
                           : next_it[-1]);
        // now all we have to do is check whether `a` is on the near side of the
        // segment from `prev` to `next` (or lies on it)
        const ll x1 = prev.x - x;
        const ll y1 = prev.y - y;
        const ll x2 = next.x - x;
        const ll y2 = next.y - y;
        if (x1 * y2 >= x2 * y1) ++result;
    }
    return result;
}
int main() {
    int t; scanf("%d", &t);
    while (t--) printf("%d\n", do_testcase());
}
