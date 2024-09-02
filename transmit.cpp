// 2024-09-02
#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>
using namespace std;
struct angle {
    int x;
    int y;
};
bool operator<(angle a1, angle a2) {
    if (a1.y > 0) {
        if (a2.y > 0) {
            return a1.x * a2.y > a1.y * a2.x;
        } else if (a2.y == 0) {
            return a2.x < 0;
        } else {
            return true;
        }
    } else if (a1.y == 0) {
        if (a1.x > 0) {
            return !(a2.y == 0 && a2.x > 0);
        } else {
            return a2.y < 0;
        }
    } else {
        if (a2.y >= 0) {
            return false;
        } else {
            return a1.x * a2.y > a1.y * a2.x;
        }
    }
}
void do_testcase(int x0, int y0, double r) {
    int N; cin >> N;
    vector<angle> angles;
    for (int i = 0; i < N; i++) {
        int x, y; cin >> x >> y;
        x -= x0; y -= y0;
        if (hypot(x, y) > r) continue;
        angles.push_back({x, y});
    }
    N = angles.size();
    sort(angles.begin(), angles.end());
    int j = 0;
    int best = 0;
    for (int i = 0; i < N; i++) {
        while ((j + 1) % N != i) {
            if (angles[i].x * angles[(j + 1) % N].y >=
                angles[i].y * angles[(j + 1) % N].x) {
                ++j;
            } else break;
        }
        int size = (j + 1 + N - i) % N;
        if (size == 0) size = N;
        if (size > best) best = size;
    }
    cout << best << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int x0, y0; double r;
    for (;;) {
        cin >> x0 >> y0 >> r;
        if (r < 0) return 0;
        do_testcase(x0, y0, r);
    }
}
