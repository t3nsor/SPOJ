// 2021-08-13

/*
Approach: First, find a face that is known to be an outer face, and pick any
edge on that face. Call the combination of a face and an edge belonging to it
a "face-edge". Then, doing a search (BFS or DFS or whatever) from the initial
face-edge, assign an orientation to all face-edges that we encounter. This
allows us to orient all outer faces of the bulk. We can then compute the
volume of the polyhedron bounded by said faces.

The bulk can contain holes, so as long as any unvisited face-edges remain
after the first step, repeatedly choose a face-edge, search from it, and
compute the volume of the hole. Subtract the volumes of all holes in order to
get the final volume of the bulk. Note that the problem description tells us
that the bulk is connected, therefore the holes can't themselves contain
islands. This guarantees that any face-edges left over after the first
paragraph belong to holes (their polyhedra's volumes must be subtracted).

Computing volume once all faces have been oriented is easy: we pick an
arbitrary reference point, which is not required to be inside the polyhedron
(for example, the origin), triangulate each face, and then sum all the signed
tetrahedron volumes. We can either take the absolute value when we're done, or
ensure that the first face-edge is oriented "correctly" (it's easier to just do
the former).

The hard part is assigning all orientations. Clearly once an orientation has
been selected for a particular face-edge, we can easily do so for all other
edges on the same face; but the question is how to get to other faces that
share the same edge. If we only need to find exact matches (i.e., one face
description has consecutive vertices A, B in some order, and another face
description does as well), it's easy to do using a hash table. But we also
have to consider the following scenario:

+--+--+
|  |  |
|  +--+
|  |  |
+--+--+

Here, what should be one face has instead been arbitrarily subdivided into 3
faces (the problem description allows this). The smaller two faces have exact
edge matches only with each other. So in order for the search to determine that
these two faces are "connected" with the large face (and other faces in the
bulk), we need to be a bit more careful. When we assign an orientation to a
particular face-edge E, let U and V be the two endpoints. Look for edges
belonging to other faces that are incident on point U and which have the same
unit direction vector as E; those are edges that we need to explore. Then, do
the same for V.

I'm not sure if pathological inputs on which the above approach fails are
possible. I haven't been able to explicitly construct one.
*/

#include <cstdlib>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

struct point { int x, y, z; };
using face = vector<point>;
// Face-edges are oriented.
struct face_edge { int f, v, d; /* d = +/- 1 */ };

// unit vector codes:
// +/- 1 = +/- \hat{x}
// +/- 2 = +/- \hat{y}
// +/- 3 = +/- \hat{z}

struct ray {
    point pt;
    int u;  // unit vector code, defined above
    friend bool operator==(const ray& r1, const ray& r2) {
        return r1.pt.x == r2.pt.x && r1.pt.y == r2.pt.y &&
               r1.pt.z == r2.pt.z && r1.u == r2.u;

    }
};
namespace std {
template <> struct hash<ray> {
    size_t operator()(const ray& r) const {
        const long long w = r.pt.x + (r.pt.y << 10) + (r.pt.z << 20);
        return std::hash<long long>{}((w << 3) + r.u);
    }
};
}

using lookup_table = unordered_multimap<ray, face_edge>;

int get_uvec(point p1, point p2) {
    if (p1.x < p2.x) return 1;
    if (p1.x > p2.x) return -1;
    if (p1.y < p2.y) return 2;
    if (p1.y > p2.y) return -2;
    if (p1.z < p2.z) return 3;
    return -3;
}

long long volume(point p1, point p2, point p3) {
    long long r1 = p1.x * (p2.y * p3.z - p2.z * p3.y);
    long long r2 = p2.x * (p3.y * p1.z - p3.z * p1.y);
    long long r3 = p3.x * (p1.y * p2.z - p1.z * p2.y);
    return r1 + r2 + r3;
}

// the indicated face-edge is given positive orientation
long long dfs(const vector<face>& faces, vector<int>& orientation,
              face_edge fe, lookup_table& M, int& unvisited) {
    if (orientation[fe.f]) {
        if (orientation[fe.f] != fe.d) {
            cerr << "inconsistent orientations for face " << fe.f << '\n';
            abort();
        }
        return 0;
    }
    orientation[fe.f] = fe.d;
    while (unvisited < faces.size() && orientation[unvisited]) ++unvisited;
    const auto& face = faces[fe.f];
    long long result = 0;
    for (int i = 0; i < face.size(); i++) {
        for (int d = -1; d <= 1; d += 2) {
            const auto j = (i + d + face.size()) % face.size();
            const auto u = get_uvec(face[i], face[j]);
            const auto range = M.equal_range({face[i], u});
            if (range.first == range.second) {
                cerr << "current edge not found in lookup table\n";
                abort();
            }
            for (auto it = range.first; it != range.second; ++it) {
                const auto& new_fe = it->second;
                if (new_fe.f == fe.f) continue;
                result += dfs(faces, orientation,
                              {new_fe.f, new_fe.v, -new_fe.d * fe.d * d},
                              M, unvisited);
            }
        }
        if (i > 0 && i + 1 < face.size()) {
            // add a triangle to the result
            result += fe.d * volume(face[0], face[i], face[i+1]);
        }
    }
    return result;
}

void do_testcase() {
    int F; cin >> F;
    vector<face> faces;
    vector<int> orientation;  // 0 = unassigned, +1/-1 = assigned
    face_edge initial_fe = {-1, -1, 1};
    int least_z = 2000;
    for (int i = 0; i < F; i++) {
        face face;
        int P; cin >> P;
        while (P--) {
            int x, y, z; cin >> x >> y >> z;
            face.push_back(point{x, y, z});
            if (z < least_z) {
                least_z = z;
                initial_fe.f = i;
                initial_fe.v = face.size() - 1;
            }
        }
        faces.push_back(move(face));
        orientation.push_back(0);
    }
    // build lookup table
    lookup_table M;
    for (int i = 0; i < faces.size(); i++) {
        const auto& face = faces[i];
        for (int j = 0; j < face.size(); j++) {
            const int k = (j + 1) % face.size();
            const auto u = get_uvec(face[j], face[k]);
            M.emplace(ray{face[j], u}, face_edge{i, j, +1});
            M.emplace(ray{face[k], -u}, face_edge{i, k, -1});
        }
    }
    int unvisited = 0;
    auto vol = abs(dfs(faces, orientation, initial_fe, M, unvisited));
    while (unvisited < faces.size()) {
        const face_edge fe = {unvisited, 0, 1};
        vol -= abs(dfs(faces, orientation, fe, M, unvisited));
    }
    cout << "The bulk is composed of " << vol/6 << " units.\n";
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) {
        do_testcase();
    }
}
