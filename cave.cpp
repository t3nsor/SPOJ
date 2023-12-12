// 2023-12-11
#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
struct HCor {
    int y;
    int x1;
    int x2;
    bool vis = false;
};
struct VCor {
    int x;
    int y1;
    int y2;
    bool vis = false;
};
void do_testcase() {
    int N; scanf("%d", &N);
    vector<HCor> H;
    vector<VCor> V;
    for (int i = 0; i < N; i++) {
        char c;
        do {
            c = getchar();
        } while (c <= 32);
        int z1, z2, z3;
        scanf("%d %d %d", &z1, &z2, &z3);
        if (z2 > z3) swap(z2, z3);
        if (c == 'H') {
            H.push_back(HCor{z1, z2, z3});
        } else {
            V.push_back(VCor{z1, z2, z3});
        }
    }
    int x, y; scanf("%d %d", &x, &y);
    char d;
    do {
        d = getchar();
    } while (d <= 32);
    // find all crossing points
    vector<vector<int>> hcross(H.size());
    vector<vector<int>> vcross(V.size());
    for (int i = 0; i < H.size(); i++) {
        for (int j = 0; j < V.size(); j++) {
            if (H[i].y >= V[j].y1 && H[i].y <= V[j].y2 &&
                V[j].x >= H[i].x1 && V[j].x <= H[i].x2) {
                hcross[i].push_back(j);
                vcross[j].push_back(i);
            }
        }
    }
    // find the corridor that we start on
    int j;
    bool inith;  // initially horizontal
    int end;
    if (d == 'E' || d == 'W') {
        for (int i = 0; i < H.size(); i++) {
            if (y == H[i].y && x >= H[i].x1 && x <= H[i].x2) {
                j = i;
                H[i].vis = true;
                break;
            }
        }
        inith = true;
        // add a fake crossing point at the start
        V.push_back(VCor{x, y, y});
        hcross[j].push_back(V.size() - 1);
        end = V.size() - 1;
    } else {
        for (int i = 0; i < V.size(); i++) {
            if (x == V[i].x && y >= V[i].y1 && y <= V[i].y2) {
                j = i;
                V[i].vis = true;
                break;
            }
        }
        inith = false;
        // ditto
        H.push_back(HCor{y, x, x});
        vcross[j].push_back(H.size() - 1);
        end = H.size() - 1;
    }
    // sort all crossing points
    for (auto& v : hcross) {
        sort(v.begin(), v.end(), [&](int i1, int i2) {
            return V[i1].x < V[i2].x;
        });
    }
    for (auto& v : vcross) {
        sort(v.begin(), v.end(), [&](int i1, int i2) {
            return H[i1].y < H[i2].y;
        });
    }
    int unvisited = N - 1;
    for (;;) {
        if (d == 'E') {
            auto it = upper_bound(hcross[j].begin(),
                                  hcross[j].end(),
                                  x,
                                  [&](int x, int i) {
                                      return x < V[i].x;
                                  });
            if (it == hcross[j].end()) {
                x = H[j].x2;
                d = 'W';
            } else {
                if (inith && *it == end) break;
                x = V[*it].x;
                if (y < V[*it].y2) {
                    d = 'N';
                    j = *it;
                } else if (x < H[j].x2) {
                    // d = 'E';
                } else {
                    d = 'S';
                    j = *it;
                }
                if (!V[*it].vis) {
                    V[*it].vis = true;
                    --unvisited;
                }
            }
        } else if (d == 'W') {
            auto it = lower_bound(hcross[j].begin(),
                                  hcross[j].end(),
                                  x,
                                  [&](int i, int x) {
                                      return V[i].x < x;
                                  });
            if (it == hcross[j].begin()) {
                x = H[j].x1;
                d = 'E';
            } else {
                --it;
                if (inith && *it == end) break;
                x = V[*it].x;
                if (y > V[*it].y1) {
                    d = 'S';
                    j = *it;
                } else if (x > H[j].x1) {
                    // d = 'W';
                } else {
                    d = 'N';
                    j = *it;
                }
                if (!V[*it].vis) {
                    V[*it].vis = true;
                    --unvisited;
                }
            }
        } else if (d == 'N') {
            auto it = upper_bound(vcross[j].begin(),
                                  vcross[j].end(),
                                  y,
                                  [&](int y, int i) {
                                      return y < H[i].y;
                                  });
            if (it == vcross[j].end()) {
                y = V[j].y2;
                d = 'S';
            } else {
                if (!inith && *it == end) break;
                y = H[*it].y;
                if (x > H[*it].x1) {
                    d = 'W';
                    j = *it;
                } else if (y < V[j].y2) {
                    // d = 'N';
                } else {
                    d = 'E';
                    j = *it;
                }
                if (!H[*it].vis) {
                    H[*it].vis = true;
                    --unvisited;
                }
            }
        } else {
            auto it = lower_bound(vcross[j].begin(),
                                  vcross[j].end(),
                                  y,
                                  [&](int i, int y) {
                                      return H[i].y < y;
                                  });
            if (it == vcross[j].begin()) {
                y = V[j].y1;
                d = 'N';
            } else {
                --it;
                if (!inith && *it == end) break;
                y = H[*it].y;
                if (x < H[*it].x2) {
                    d = 'E';
                    j = *it;
                } else if (y > V[j].y1) {
                    // d = 'S';
                } else {
                    d = 'W';
                    j = *it;
                }
                if (!H[*it].vis) {
                    H[*it].vis = true;
                    --unvisited;
                }
            }
        }
    }
    printf("%d\n", unvisited);
}
int main() {
    int T; scanf("%d", &T);
    while (T--) do_testcase();
}
