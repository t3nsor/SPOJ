// 2023-11-08
// Implementation of "gift wrapping" algorithm in three dimensions. The idea is
// that if you have an edge (p1, p2) that you know is on the hull, then you can
// find a point `i` that is part of a face to which `p1` and `p2` belong by
// looping over all points and picking the one that is "in front of" all other
// points, where we say that `i1` is in front of `i2` if, when we orient
// ourselves so that p1 -> p2 -> i2 is a counterclockwise turn in the plane of
// the page, then `i1` is in front of the page. The devil is in the details,
// which are explained in comments below.
#include <algorithm>
#include <map>
#include <math.h>
#include <stack>
#include <stdio.h>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;
struct Point {
    int x;
    int y;
    int z;
};
struct MyMap : map<pair<int, int>, int> {
    int& get(int x, int y) {
        if (x > y) std::swap(x, y);
        return operator[]({x, y});
    }
};
using ll = long long;
// Compares the vectors (x1, y1) and (x2, y2), where `x1` and `x2` are both
// nonnegative, by comparing the counterclockwise angles from the positive
// x-axis to each point (this is basically like comparing the `atan2` functions,
// but avoids any floating point math). The zero vector is considered
// to point in the direction of the positive x-axis.
int compare_angle_pos(int x1, int y1, int x2, int y2) {
    if (x1 == 0 && y1 == 0) x1 = 1;
    if (x2 == 0 && y2 == 0) x2 = 1;
    const ll p1 = y1 * ll(x2);
    const ll p2 = y2 * ll(x1);
    if (p1 > p2) return 1; else if (p1 < p2) return -1; else return 0;
}
// Compares the vectors (x1, y1) and (x2, y2) by comparing the counterclockwise
// angles from the positive x-axis to each point (this is basically like
// comparing the `atan2` functions, but avoids any floating point math). The
// zero vector is considered to point in the direction of the positive x-axis.
int compare_angle(int x1, int y1, int x2, int y2) {
    const bool neg1 = y1 < 0 || (y1 == 0 && x1 < 0);
    const bool neg2 = y2 < 0 || (y2 == 0 && x2 < 0);
    if (neg1) {
       if (neg2) {
           return -compare_angle_pos(-x1, -y1, -x2, -y2);
       } else {
           return 1;
       }
    } else {
        if (neg2) {
            return -1;
        } else {
            return compare_angle_pos(x1, y1, x2, y2);
        }
    }
}
// Returns the sign of the volume of the parallelepiped defined by the vectors
// (b - a, c - a, d - a) (i.e. the sign of that determinant).
int volumesgn(Point a, Point b, Point c, Point d) {
    using i128 = __int128;
    b.x -= a.x;
    b.y -= a.y;
    b.z -= a.z;
    c.x -= a.x;
    c.y -= a.y;
    c.z -= a.z;
    d.x -= a.x;
    d.y -= a.y;
    d.z -= a.z;
    const i128 det = b.x * (c.y * i128(d.z) - c.z * i128(d.y)) +
                     b.y * (c.z * i128(d.x) - c.x * i128(d.z)) +
                     b.z * (c.x * i128(d.y) - c.y * i128(d.x));
    if (det > 0) return 1; else if (det < 0) return -1; else return 0;
}
double hypot3(double x, double y, double z) {
    return sqrt(x*x + y*y + z*z);
}
void do_testcase() {
    int N; scanf("%d", &N);
    int p1 = 0;
    vector<Point> P;
    // We have to bootstrap the gift wrap by finding the first edge. First, we
    // find a point that's guaranteed to be on the hull. If all coordinate
    // values are distinct then this is easy: just pick the one with the
    // smallest z-coordinate. Basically all the difficulty in this problem has
    // to do with what happens when there are ties. Here, we break ties by
    // x-coordinate and then y-coordinate. This guarantees that the chosen point
    // will be in the minimal set of points that generate the convex hull. (We
    // avoid any non-minimal points since they cause issues with the algorithm.)
    for (int i = 0; i < N; i++) {
        double x, y, z; scanf("%lf %lf %lf", &x, &y, &z);
        // For speed & accuracy, do the computations using only ints:
        // multiply by 2e7 and round. Eventually we'll scale back down by 2e7
        // when computing the final result, which means each coordinate will be
        // changed by at most 2.5e-8. It can be shown that this will give a
        // final volume that's within 0.01 of the correct value.
        x = round(20000000.0 * x);
        y = round(20000000.0 * y);
        z = round(20000000.0 * z);
        P.push_back(Point{(int)x, (int)y, (int)z});
        if (tie(P[i].z, P[i].x, P[i].y) < tie(P[p1].z, P[p1].x, P[p1].y)) {
            p1 = i;
        }
    }
    // Having found `p1`, which is known to be on the minimal hull, we now find
    // a point `p2` that is guaranteed to have an edge to `p1` in the minimal
    // hull. We do this by minimizing the slope (i.e. dz/dx) of the projection
    // of the ray from p1 to p2 onto the xz plane, where 0/0 is defined to be 0.
    // In case of a tie, where points p2 and p3 both lie on the plane that makes
    // the minimal angle with the x axis, we pick the point where the projection
    // of the ray onto that plane makes the smallest clockwise angle from the
    // positive y-axis. If there's still a tie, we pick the point furthest from
    // p1.
    int p2 = !p1;
    for (int i = 0; i < N; i++) {
        if (i == p1 || i == p2) continue;

        const Point d2 = {P[p2].x - P[p1].x,
                          P[p2].y - P[p1].y,
                          P[p2].z - P[p1].z};
        const Point di = {P[i].x - P[p1].x,
                          P[i].y - P[p1].y,
                          P[i].z - P[p1].z};

        const int c1 = compare_angle(di.x, di.z, d2.x, d2.z);
        if (c1 < 0) {
            p2 = i;
            continue;
        } else if (c1 > 0) {
            continue;
        }

        const int c2 = compare_angle(di.y, abs(di.x) + abs(di.z),
                                     d2.y, abs(d2.x) + abs(d2.z));
        if (c2 < 0) {
            p2 = i;
            continue;
        } else if (c2 > 0) {
            continue;
        }

        if (abs(di.x) + abs(di.y) + abs(di.z) >
            abs(d2.x) + abs(d2.y) + abs(d2.z)) {
            p2 = i;
        }
    }

    stack<pair<int, int>> stk;
    MyMap M;
    vector<tuple<int, int, int>> faces;
    // Points p1 and p2 are part of the hull.
    stk.emplace(p1, p2);
    M.get(p1, p2) = 1;
    while (!stk.empty()) {
        const auto edge = stk.top();
        p1 = edge.first;
        p2 = edge.second;
        stk.pop();
        if (M.get(p1, p2) >= 2) {
            // This edge is already part of 2 faces, so there's nothing left to
            // do.
            continue;
        }
        vector<int> nextv;
        for (int i = 0; i < N; i++) {
            if (i == p1 || i == p2) continue;
            if (nextv.empty()) {
                nextv.push_back(i);
                continue;
            }
            // Check whether `P[i]` is "in front of" `P[nextv[0]]`.
            const int s = volumesgn(P[p1], P[p2], P[nextv[0]], P[i]);
            if (s > 0) {
                nextv = {i};
            } else if (s == 0) {
                nextv.push_back(i);
            }
        }
        // If there's a tie, then we have a face with 4 or more vertices. We do
        // a 2d gift wrap to process the entire face at once. The idea is that
        // we want to pick the point `i` such that the angle `p2` -> `p1` -> `i`
        // is as large as possible, then pick `j` such that `p1` -> `i` -> `j`
        // is as large as possible, and so on until we get back to `p2`. But
        // this is a bit expensive to calculate directly so instead we take the
        // cross product (i2 - p2) x (i1 - p2) to compare two potential next
        // points i1, i2 with each other. This always gives a vector that's
        // perpendicular to the plane that p1, p2, i1, and i2 lie in, and we
        // arbitrarily pick the first nonzero coordinate of that vector to
        // determine whether we consider this cross product "positive" or
        // "negative". Note that if there's still a tie, we always pick a
        // point that is as far away as possible, to avoid non-minimal points.
        // (This code would be way simpler if the given points were in "general
        // position", i.e., no 4 points coplanar.)
        nextv.push_back(p1);
        nextv.push_back(p2);
        vector<int> chain;
        int back = p2;
        for (;;) {
            int next = -1;
            for (const int i : nextv) {
                if (i == back || (back == p2 && i == p1)) continue;
                if (next == -1) {
                    next = i;
                    continue;
                }
                const Point d1i = {P[i].x - P[back].x,
                                   P[i].y - P[back].y,
                                   P[i].z - P[back].z};
                const Point d1n = {P[next].x - P[back].x,
                                   P[next].y - P[back].y,
                                   P[next].z - P[back].z};
                const auto crsx = ll(d1i.y) * d1n.z - ll(d1i.z) * d1n.y;
                const auto crsy = ll(d1i.z) * d1n.x - ll(d1i.x) * d1n.z;
                const auto crsz = ll(d1i.x) * d1n.y - ll(d1i.y) * d1n.x;
                if (crsx > 0) {
                    next = i;
                    continue;
                } else if (crsx < 0) {
                    continue;
                } else if (crsy > 0) {
                    next = i;
                    continue;
                } else if (crsy < 0) {
                    continue;
                } else if (crsz > 0) {
                    next = i;
                    continue;
                } else if (crsz < 0) {
                    continue;
                }
                // If there's still a tie, pick the point that's further away.
                if (abs(d1i.x) + abs(d1i.y) + abs(d1i.z) >
                    abs(d1n.x) + abs(d1n.y) + abs(d1n.z)) {
                    next = i;
                }
            }
            back = next;
            if (next == p2) {
                // If this gift wrap gets us back to the point we started at,
                // then we went in the wrong direction.
                reverse(chain.begin(), chain.end());
                break;
            } else if (next == p1) {
                // If we started at `p2` and ended at `p1`, we went in the right
                // direction.
                break;
            }
            chain.push_back(next);
        }
        // The correct counterclockwise ordering of intermediate vertices should
        // now be in `chain`, and `chain.size()` is the number of triangular
        // faces to create. Note that when pushing edges onto the stack, the
        // order is crucial. If you get the order wrong here, then when such an
        // edge gets processed, we end up picking the point we just came from.
        faces.emplace_back(p1, p2, chain.front());
        stk.emplace(chain.front(), p2);
        M.get(chain.front(), p2)++;
        for (int i = 1; i < chain.size(); i++) {
            faces.emplace_back(p1, chain[i - 1], chain[i]);
            stk.emplace(chain[i], chain[i - 1]);
            M.get(chain[i], chain[i - 1])++;
        }
        stk.emplace(p1, chain.back());
        M.get(p1, chain.back())++;
        if (faces.size() > 2 * N - 4) {
            // there must be a logic error, since 2N - 4 is the maximum possible
            // number of faces that the convex hull can have :(
            throw;
        }
    }
    // Now prepare to calculate the surface are and volume. First, scale the
    // coordinates back down to their original values.
    vector<double> X(N);
    vector<double> Y(N);
    vector<double> Z(N);
    double Ox = 0, Oy = 0, Oz = 0;
    for (int i = 0; i < N; i++) {
        X[i] = P[i].x; X[i] /= 20000000.0;
        Y[i] = P[i].y; Y[i] /= 20000000.0;
        Z[i] = P[i].z; Z[i] /= 20000000.0;
        Ox += X[i];
        Oy += Y[i];
        Oz += Z[i];
    }
    // To compute the volume, we pick a point that is definitely inside the
    // convex hull (luckily, the centroid of all input points is always such a
    // point) and sum the (unsigned) volumes of the tetrahedra formed by that
    // point and each face.
    Ox /= N;
    Oy /= N;
    Oz /= N;
    double sa = 0, v = 0;
    for (const auto& face : faces) {
        const int i = get<0>(face);
        const int j = get<1>(face);
        const int k = get<2>(face);
        const double bx = X[j] - X[i];
        const double by = Y[j] - Y[i];
        const double bz = Z[j] - Z[i];
        const double cx = X[k] - X[i];
        const double cy = Y[k] - Y[i];
        const double cz = Z[k] - Z[i];
        sa += hypot3(bx * cy - by * cx,
                     by * cz - bz * cy,
                     bz * cx - bx * cz) / 2.0;
        const double dx = Ox - X[i];
        const double dy = Oy - Y[i];
        const double dz = Oz - Z[i];
        v += fabs(bx * (cy * dz - cz * dy) +
                  by * (cz * dx - cx * dz) +
                  bz * (cx * dy - cy * dx)) / 6.0;
    }
    printf("%f %f\n", sa, v);
}
int main() {
    int T; scanf("%d", &T);
    while(T--) do_testcase();
}
