// 2018-12-18
#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
int elev[100][100];
int ht[100][100];
const int dr[4] = {0, 1, 0, -1};
const int dc[4] = {1, 0, -1, 0};
struct S {
    int pri;
    int i;
    int j;
    bool operator>(const S& other) const {
        return pri > other.pri;
    }
};
int main() {
    ios::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) {
        memset(ht, 0, sizeof(ht));
        int R, C;
        cin >> R >> C;
        int res = 0;
        priority_queue<S, vector<S>, greater<S>> q;
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++) {
                cin >> elev[i][j];
                if (i == 0 || i == R - 1 || j == 0 || j == C - 1) {
                    q.push(S{elev[i][j], i, j});
                }
            }
        }
        while (!q.empty()) {
            const auto s = q.top();
            q.pop();
            if (ht[s.i][s.j]) continue;
            if (s.pri > elev[s.i][s.j]) {
                ht[s.i][s.j] = s.pri;
                res += s.pri - elev[s.i][s.j];
            } else {
                ht[s.i][s.j] = elev[s.i][s.j];
            }
            for (int i = 0; i < 4; i++) {
                const int i2 = s.i + dr[i];
                const int j2 = s.j + dc[i];
                if (i2 < 0 || i2 >= R || j2 < 0 || j2 >= C) continue;
                q.push(S{ht[s.i][s.j], i2, j2});
            }
        }
        cout << res << '\n';
    }
}
