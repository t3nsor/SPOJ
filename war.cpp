// 2026-08-31
// The algorithm works in two stages.  In the first stage, each border segment
// is split into two directed segments (we'll use the abbreviation "DS" for
// directed segment), and DSes are grouped together into polygons; a polygon is
// considered to be the sequence of DSes that make up its boundary in a
// counterclockwise direction.  Notice that if a DS and its opposite DS (i.e.
// two DSes generated originally from a single input segment) belong to
// different polygons, then those two polygons are states that are at war.  In
// the second stage, the capital cities are matched to the polygons found in the
// first stage.  To determine whether a city lies within a state, we use the
// standard raycasting algorithm.  A state can be completely surrounded by
// another state, in which case the inner state's capital city will be contained
// within both polygons (see second sample case).  The trickiest case, however,
// is something like
//
// xxxxxxxxxxxxx
// x           x
// x xxxxxxxxx x
// x xx     xx x
// x x x   x x x
// x x  xxx  x x
// x x  x!x  x x
// x x  xxx  x x
// x x x   x x x
// x xx     xx x
// x xxxxxxxxx x
// x           x
// xxxxxxxxxxxxx
//
// where x's denote edges and the ! character denotes the capital of the
// innermost state.  This counterexample (found by someone I asked for help
// after I kept getting WA) shows that when a capital city is contained within
// two or more polygons it's not correct to assume that there the smallest one
// borders the next smallest, and so on.  Instead, two states that both contain
// a given capital point can only be assumed to border each other if they are
// consecutive in the list of enclosing states sorted by area **and** the
// smaller of the two has at least one edge for which the state on the other
// side isn't already known from the first stage.
#include <algorithm>
#include <map>
#include <random>
#include <set>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
using LL = long long;
/// Return `true` if the vector (x, y) has direction in [0, pi).
bool firsthalf(LL x, LL y) {
    return y > 0 || (y == 0 && x > 0);
}
bool angle_compare(LL x1, LL y1, LL x2, LL y2) {
    const bool f1 = firsthalf(x1, y1);
    const bool f2 = firsthalf(x2, y2);
    if (f1 != f2) return f1;
    if (!f1) {
        x1 = -x1;
        y1 = -y1;
        x2 = -x2;
        y2 = -y2;
    }
    return x1*y2 > x2*y1;
}
struct Vec {
    LL x;
    LL y;
    friend bool operator<(const Vec& v1, const Vec& v2) {
        return angle_compare(v1.x, v1.y, v2.x, v2.y);
    }
};
struct Point {
    int x;
    int y;
    friend bool operator==(const Point& p1, const Point& p2) {
        return p1.x == p2.x && p1.y == p2.y;
    }
    friend bool operator<(const Point& p1, const Point& p2) {
        return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
    }
    friend Vec operator-(const Point& p1, const Point& p2) {
        return {LL(p1.x) - p2.x, LL(p1.y) - p2.y};
    }
};
struct DirSeg {
    Point p1;
    Point p2;
    friend bool operator<(const DirSeg& ds1, const DirSeg& ds2) {
        return ds1.p1 < ds2.p1 || (ds1.p1 == ds2.p1 && ds1.p2 < ds2.p2);
    }
};
LL det(const Vec& v1, const Vec& v2) {
    return v1.x*v2.y - v2.x*v1.y;
}
// returns:
// 2 if the ray `ray` from point `O` contains `P` or `Q`
// 0 if the ray does not intersect the segment from `P` to `Q`
// +1 if they intersect and `O` is to the left of the ray from `P` to `Q`
// -1 if ...                              right ...
// The behavior is undefined if O lies on the segment PQ.
int classify(Point O, Vec ray, Point P, Point Q) {
    const Vec OP = P - O;
    const Vec OQ = Q - O;
    const LL d1 = det(ray, OP);
    const LL d2 = det(ray, OQ);
    if (d1 == 0 || d2 == 0) return 2;  // inconclusive, generate new ray
    if ((d1 > 0) == (d2 > 0)) return 0;  // line containing `ray` does not
                                         // intersect segment PQ
    // line intersects segment, but on the correct side?
    const LL d3 = det(OP, OQ);
    if (d3 == 0) return 0;  // line through PQ passes through O
    if ((d3 > 0) != (d2 > 0)) return 0;  // wrong side
    if (d2 > 0) return 1; else return -1;
}
void do_testcase(int n, int m) {
    vector<Point> capital(n);
    for (int i = 0; i < n; i++) scanf("%d %d", &capital[i].x, &capital[i].y);
    set<DirSeg> dirsegs;
    map<Point, vector<Vec>> edges_for_point;
    while (m--) {
        Point p1, p2;
        scanf("%d %d %d %d", &p1.x, &p1.y, &p2.x, &p2.y);
        dirsegs.insert(DirSeg{p1, p2});
        dirsegs.insert(DirSeg{p2, p1});
        edges_for_point[p1].push_back(p2 - p1);
        edges_for_point[p2].push_back(p1 - p2);
    }
    for (auto& kv : edges_for_point) sort(kv.second.begin(), kv.second.end());
    vector<vector<DirSeg>> polys;
    vector<pair<LL, int>> area;
    // Repeatedly select an unassigned segment (arbitrarily).  Then, determine
    // the region it bounds by walking around in a counterclockwise direction,
    // i.e., pick the outward edge from the destination point that is as close
    // as possible in the clockwise direction to the current edge and repeat
    // until we get back to our starting point.
    while (!dirsegs.empty()) {
        auto cur = *dirsegs.begin();
        const auto initp = cur.p1;
        vector<DirSeg> poly;
        for (;;) {
            poly.push_back(cur);
            dirsegs.erase(cur);
            if (cur.p2 == initp) break;
            Vec rev = cur.p1 - cur.p2;
            auto& l = edges_for_point[cur.p2];
            auto it = lower_bound(l.begin(), l.end(), rev);
            if (it == l.begin()) it = l.end();
            const auto nextvec = it[-1];
            const Point p3 = {int(cur.p2.x + nextvec.x),
                              int(cur.p2.y + nextvec.y)};
            cur = DirSeg{cur.p2, p3};
        }
        // check whether the polygon is positively oriented
        LL a = 0;
        // note that intermediate values of `a` can overflow but the final
        // result will be twice the actual area so it should fit in `long long`
        for (const auto& ds : poly) {
            a += LL(ds.p1.x) * ds.p2.y - LL(ds.p2.x) * ds.p1.y;
        }
        // discard negatively oriented polygons (they "enclose" the unbounded
        // area of the wasteland)
        if (a < 0) continue;
        area.emplace_back(a, polys.size());
        polys.push_back(move(poly));
    }
    // sort the polygons by area
    sort(area.begin(), area.end());
    {
        vector<vector<DirSeg>> tmp;
        for (int i = 0; i < area.size(); i++) {
            tmp.push_back(move(polys[area[i].second]));
        }
        polys = move(tmp);
    }

    vector<vector<char>> adj(polys.size(), vector<char>(polys.size(), 0));

    // generate map from segments to polygons (needed in the step below)
    map<DirSeg, int> polyid;
    for (int i = 0; i < polys.size(); i++) {
        for (const auto& ds : polys[i]) polyid[ds] = i;
    }

    // For each directed segment, if it and its reverse are both boundaries of
    // states, then those two states are adjacent.  Also, if a state has any
    // edge for which we don't know the state on the other side, we have to make
    // a note of that because we'll use it in the next step.
    vector<char> could_be_surrounded(polys.size(), 0);
    for (const auto& kv : polyid) {
        const auto& dirseg = kv.first;
        const auto id1 = kv.second;
        const auto it = polyid.find({dirseg.p2, dirseg.p1});
        if (it == polyid.end()) {
            could_be_surrounded[id1] = 1;
        } else {
            const auto id2 = it->second;
            adj[id1][id2] = 1;
        }
    }

    vector<int> poly2cap(polys.size(), -1);
    vector<int> cap2poly(polys.size());
    // Use the naive algorithm for finding which polygon each capital belongs
    // to: if the capital is inside a polygon then a randomly generated ray from
    // the capital should intersect the polygon exactly once, where we consider
    // a crossing from outside to inside to be worth -1 intersection.  Note that
    // if a polygon P1 is contained within a polygon P2 then the actual state
    // corresponding to P2 excludes the area within P1, so the actual state to
    // which a capital belongs is the smallest enclosing polygon.
    mt19937 gen(12345);
    uniform_int_distribution<int> dist(-1e9, 1e9);
    for (int i = 0; i < n; i++) {
        vector<int> enclosing;
        for (int j = 0; j < polys.size(); j++) {
retry:
            Vec ray{dist(gen), dist(gen)};
            if (ray.x == 0 && ray.y == 0) goto retry;
            int acc = 0;
            for (const auto& ds : polys[j]) {
                int c = classify(capital[i], ray, ds.p1, ds.p2);
                if (c == 2) goto retry;
                acc += c;
            }
            if (acc == 1) enclosing.push_back(j);
        }
        cap2poly[i] = enclosing[0];
        poly2cap[enclosing[0]] = i;
        for (int j = 1; j < enclosing.size(); j++) {
            if (!could_be_surrounded[enclosing[j - 1]]) continue;
            adj[enclosing[j - 1]][enclosing[j]] = 1;
            adj[enclosing[j]][enclosing[j - 1]] = 1;
        }
    }
    for (int i = 0; i < n; i++) {
        const int p = cap2poly[i];
        vector<int> enemies;
        for (int j = 0; j < n; j++) {
            if (adj[p][j]) enemies.push_back(poly2cap[j]);
        }
        sort(enemies.begin(), enemies.end());
        printf("%d", (int)enemies.size());
        for (const auto j : enemies) {
            printf(" %d", j + 1);
        }
        putchar('\n');
    }
}
int main() {
    for (;;) {
        int n, m; scanf("%d %d", &n, &m);
        if (n + m == 0) break;
        do_testcase(n, m);
    }
}
