// 2020-08-18
// This is a very tricky problem, and I wouldn't have been able to get it
// accepted without looking at the test data.

#include <cstdlib>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

constexpr int dr[4] = {1, 0, -1, 0};
constexpr int dc[4] = {0, 1, 0, -1};

// fields are labelled from 0 to R-1 and 0 to C-1
struct loc {
    int r;
    int c;
    static int R;
    static int C;
};
int loc::R;
int loc::C;

struct point {
    int x;
    int y;
};

// returns the centre point for the given field
inline point point_from_loc(loc l) {
    return {2 * l.c + 1, 2 * (loc::R - l.r) - 1};
}

bool compute_visibilityx(const vector<vector<int>>& ht,
                         loc l, loc target) {
    if (l.c == target.c) return true;
    if (l.c > target.c) swap(l, target);
    const auto lp = point_from_loc(l);
    const auto tp = point_from_loc(target);
    const int lz = 2 * ht[l.r][l.c] + 1;
    const int tz = 2 * ht[target.r][target.c] + 1;
    // iterate from l.r to target.r, and at each step, check whether the
    // segment intersects the interior of a yz face whose x-coordinate is on
    // the *far* side of the column (i.e., in the direction of `step`)
    for (int j = l.c, x = lp.x + 1; j != target.c; j++, x += 2) {
        // in the reals, the relevant y-coordinate is:
        //   y = lp.y + (x - lp.x) / (tp.x - lp.x) * (tp.y - lp.y)
        // we need to divide this by 2 and round down in order to find the
        // bottom y-coordinate of the corresponding field
        const int dividend = lp.y * (tp.x - lp.x) + (x - lp.x) * (tp.y - lp.y);
        const int divisor = 2 * (tp.x - lp.x);
        const auto div_result = div(dividend, divisor);
        const int r = loc::R - div_result.quot - 1;
        int z_min;
        if (div_result.rem == 0) {
            // The beam goes through the corner. In this case we have to look
            // at two diagonally adjacent fields (the other two don't matter,
            // as the beam can squeeze through the corner). But to know which
            // ones we have to use, we need to know whether y is increasing or
            // decreasing along this path.
            if (tp.y >= lp.y) {
                z_min = 2 * max(ht[r+1][j], ht[r][j+1]);
            } else {
                z_min = 2 * max(ht[r][j], ht[r+1][j+1]);
            }
        } else {
            // The beam goes through the interior of a face.
            z_min = 2 * max(ht[r][j], ht[r][j+1]);
        }
        // the z-coordinate of the intersection is:
        //   z = lz + (x - lp.x) / (tp.x - lp.x) * (tz - lz)
        // we need to check whether this is >= z_min.
        if ((z_min - lz) * (tp.x - lp.x) > (x - lp.x) * (tz - lz)) {
            return false;
        }
    }
    return true;
}

inline bool compute_visibilityy(const vector<vector<int>>& ht,
                                loc l, loc target) {
    if (l.r == target.r) return true;
    if (l.r < target.r) swap(l, target);
    const auto lp = point_from_loc(l);
    const auto tp = point_from_loc(target);
    const int lz = 2 * ht[l.r][l.c] + 1;
    const int tz = 2 * ht[target.r][target.c] + 1;
    for (int i = l.r, y = lp.y + 1; i != target.r; i--, y += 2) {
        // in the reals, the relevant x-coordinate is:
        //   x = lp.x + (y - lp.y) / (tp.y - lp.y) * (tp.x - lp.x)
        // we need to divide this by 2 and round down in order to find the
        // left x-coordinate of the corresponding field
        const int dividend = lp.x * (tp.y - lp.y) + (y - lp.y) * (tp.x - lp.x);
        const int divisor = 2 * (tp.y - lp.y);
        const auto div_result = div(dividend, divisor);
        const int c = div_result.quot;
        int z_min;
        if (div_result.rem == 0) {
            // The beam goes through the corner. In this case we have to look
            // at two diagonally adjacent fields (the other two don't matter,
            // as the beam can squeeze through the corner). But to know which
            // ones we have to use, we need to know whether x is increasing or
            // decreasing along this path.
            if (tp.x >= lp.x) {
                z_min = 2 * max(ht[i][c-1], ht[i-1][c]);
            } else {
                z_min = 2 * max(ht[i-1][c-1], ht[i][c]);
            }
            continue;
        } else {
            // The beam goes through the interior of a face.
            z_min = 2 * max(ht[i][c], ht[i-1][c]);
        }
        // the z-coordinate of the intersection is:
        //   z = lz + (y - lp.y) / (tp.y - lp.y) * (tz - lz)
        // we need to check whether this is >= z_min.
        if ((z_min - lz) * (tp.y - lp.y) > (y - lp.y) * (tz - lz)) {
            return false;
        }
    }
    return true;
}

inline bool compute_visibility1(const vector<vector<int>>& ht,
                                loc l, loc target) {
    // We can ignore possible intersections with the interior of faces parallel
    // to the xy plane (xy faces) or with the interior of edges that are
    // parallel to the x or y axes (x or y edges). The reason is that if the
    // line segment between the two points does enter the ground via such an
    // intersection, it has to exit via a different type of intersection (i.e.,
    // with the interior of an xz face, yz face, or z edge) as both the initial
    // and final points are above ground.
    //
    // To detect such intersections, the idea is to first walk in the positive
    // x-direction from one point to the other, looking at all the yz planes
    // that occur in between at even x-coordinates. At each such x-coordinate,
    // we compute the y-coordinate where the intersection occurs. This
    // determines which yz faces or z edges the line segment might intersect
    // the interior of at this x-coordinate (assuming infinite height). Then we
    // check whether the segment at this point is high enough to actuall avoid
    // the yz faces / z edges. After completing the walk along the x-axis, we
    // repeat the same check by walking along the y-axis to look for
    // intersections with xz faces / z edges.
    return compute_visibilityx(ht, l, target) &&
           compute_visibilityy(ht, l, target);
}

inline bool compute_visibility(const vector<vector<int>>& ht,
                               loc l1, loc l2, loc target) {
    return compute_visibility1(ht, l1, target) ||
           compute_visibility1(ht, l2, target);
}

void do_testcase() {
    cin >> loc::R >> loc::C;
    vector<vector<int>> ht;
    // note: -1 means the square is impassable
    vector<vector<int>> dist(loc::R, vector<int>(loc::C, 1000000));
    for (int i = 0; i < loc::R; i++) {
        ht.emplace_back();
        for (int j = 0; j < loc::C; j++) {
            int h; cin >> h;
            ht.back().push_back(h);
        }
    }
    loc from, to;
    cin >> from.r >> from.c >> to.r >> to.c;
    --from.r; --from.c; --to.r; --to.c;
    queue<loc> q;
    dist[from.r][from.c] = 0;
    q.push(from);
    while (!q.empty()) {
        const auto cur = q.front();
        q.pop();
        if (cur.r == to.r && cur.c == to.c) {
            cout << "The shortest path is " << dist[to.r][to.c] <<
                    " steps long.\n";
            return;
        }
        for (int k = 0; k < 4; k++) {
            const loc next = {cur.r + dr[k], cur.c + dc[k]};
            if (next.r < 0 || next.r >= loc::R ||
                next.c < 0 || next.c >= loc::C) continue;
            if (dist[next.r][next.c] < 0) continue;  // not visible
            if (dist[next.r][next.c] < 1000000) continue;  // already visited
            if (ht[next.r][next.c] > ht[cur.r][cur.c] + 1 ||
                ht[next.r][next.c] < ht[cur.r][cur.c] - 3) continue;
            bool vis = compute_visibility(ht, from, to, next);
            if (vis) {
                dist[next.r][next.c] = dist[cur.r][cur.c] + 1;
                q.push(next);
            } else {
                dist[next.r][next.c] = -1;
            }
        }
    }
    cout << "Mission impossible!\n";
}

int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) {
        do_testcase();
    }
}
