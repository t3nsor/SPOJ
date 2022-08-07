// 2022-08-07
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
int dx[4]    = { 0,   1,   0,  -1};
int dy[4]    = { 1,   0,  -1,   0};
char name[4] = {'N', 'E', 'S', 'W'};
int get_dir(char name_) {
    int result = 0;
    while (name[result] != name_) ++result;
    return result;
}
void do_testcase() {
    int N; cin >> N;
    int x = 0, y = 0;
    unordered_map<int, map<int, int>> xylookup;
    unordered_map<int, map<int, int>> yxlookup;
    xylookup[0][0] = 0;
    yxlookup[0][0] = 0;
    string s; cin >> s;
    for (int i = 1; i <= N; i++) {
        int dir = 0;
        while (s[i-1] != name[dir]) ++dir;
        x += dx[dir];
        y += dy[dir];
        xylookup[x][y] = i;
        yxlookup[y][x] = i;
    }
    int shortest = 1000000;
    int b, e, dir;
    x = 0; y = 0;
    for (int i = 0; i < N; i++) {
        int fwd = get_dir(s[i]);
        int bwd = i > 0 ? (get_dir(s[i-1]) + 2) % 4 : -1;
        for (int j = 0; j < 4; j++) {
            if (j == fwd || j == bwd) continue;
            if (j == 0) {
                // look to the north
                const auto& m = xylookup[x];
                auto it = m.upper_bound(y);
                if (it == m.end()) continue;
                if (it->second < i) continue;
                const int dist = it->first - y;
                if (dist < shortest ||
                    (dist == shortest && b == i && e < it->second)) {
                    shortest = dist;
                    b = i;
                    e = it->second;
                    dir = 0;
                }
            } else if (j == 1) {
                // look to the east
                const auto& m = yxlookup[y];
                auto it = m.upper_bound(x);
                if (it == m.end()) continue;
                if (it->second < i) continue;
                const int dist = it->first - x;
                if (dist < shortest ||
                    (dist == shortest && b == i && e < it->second)) {
                    shortest = dist;
                    b = i;
                    e = it->second;
                    dir = 1;
                }
            } else if (j == 2) {
                // look to the south
                const auto& m = xylookup[x];
                auto it = m.find(y);
                if (it == m.begin()) continue;
                --it;
                if (it->second < i) continue;
                const int dist = y - it->first;
                if (dist < shortest ||
                    (dist == shortest && b == i && e < it->second)) {
                    shortest = dist;
                    b = i;
                    e = it->second;
                    dir = 2;
                }
            } else {
                // look to the west
                const auto& m = yxlookup[y];
                auto it = m.find(x);
                if (it == m.begin()) continue;
                --it;
                if (it->second < i) continue;
                const int dist = x - it->first;
                if (dist < shortest ||
                    (dist == shortest && b == i && e < it->second)) {
                    shortest = dist;
                    b = i;
                    e = it->second;
                    dir = 3;
                }
            }
        }
        x += dx[fwd];
        y += dy[fwd];
    }
    cout << shortest << ' ' << b << ' ' << e << ' ' << name[dir] << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) do_testcase();
}
