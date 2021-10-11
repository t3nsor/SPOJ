// 2021-10-11
#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <string>
using namespace std;
constexpr int X = 250;
constexpr int Y = 250;
constexpr int Z = 30;
int a[X+1][Y+1][Z+1];
int clamp(int x, int m, int M) {
    return max(m, min(M, x));
}
void cut(int x1, int y1, int z1, int x2, int y2, int z2) {
    x1 = clamp(x1, 0, X);
    x2 = clamp(x2, 0, X);
    y1 = clamp(y1, 0, Y);
    y2 = clamp(y2, 0, Y);
    z1 = clamp(z1, 0, Z);
    z2 = clamp(z2, 0, Z);
    a[x1][y1][z1]++;
    a[x1][y1][z2]--;
    a[x1][y2][z1]--;
    a[x1][y2][z2]++;
    a[x2][y1][z1]--;
    a[x2][y1][z2]++;
    a[x2][y2][z1]++;
    a[x2][y2][z2]--;
}
int get_num(const string& s, int& i) {
    bool minus = false;
    if (s[i] == '-') {
        minus = true;
        i++;
    } else if (s[i] == '+') {
        i++;
    }
    int val = 0;
    while (i < s.size() && isdigit(s[i])) {
        val = 10 * val + s[i] - '0';
        i++;
    }
    return minus ? -val : val;
}
void do_testcase() {
    int x = 0;
    int y = 0;
    int z = -1;  // blade cuts from z - 30 to z
    string s; cin >> s;
    int i = 0;
    while (i < s.size()) {
        if (s[i] == '[' || s[i] == ']' || s[i] == ';') {
            i++;
            continue;
        }
        if (s[i] == '^') {
            i++;
            const int val = get_num(s, i);
            const int minz = min(z, z + val);
            const int maxz = max(z, z + val);
            z += val;
            cut(x, y, minz - 30, x + 1, y + 1, maxz);
        } else {
            if (s[i++] != '@') throw;
            const char dir = s[i++];
            if (dir == 'N' || dir == 'S') {
                const int dy = get_num(s, i) * (dir == 'S' ? 1 : -1);
                const int miny = min(y, y + dy);
                const int maxy = max(y, y + dy);
                y += dy;
                cut(x, miny, z - 30, x + 1, maxy + 1, z);
            } else {
                const int dx = get_num(s, i) * (dir == 'E' ? 1 : -1);
                const int minx = min(x, x + dx);
                const int maxx = max(x, x + dx);
                x += dx;
                cut(minx, y, z - 30, maxx + 1, y + 1, z);
            }
        }
    }
    for (int i = 0; i <= X; i++) {
        for (int j = 0; j <= Y; j++) {
            for (int k = 1; k <= Z; k++) {
                a[i][j][k] += a[i][j][k-1];
            }
        }
    }
    for (int i = 0; i <= X; i++) {
        for (int j = 1; j <= Y; j++) {
            for (int k = 0; k <= Z; k++) {
                a[i][j][k] += a[i][j-1][k];
            }
        }
    }
    int result = 0;
    for (int i = 1; i <= X; i++) {
        for (int j = 0; j <= Y; j++) {
            for (int k = 0; k <= Z; k++) {
                a[i][j][k] += a[i-1][j][k];
                if (a[i-1][j][k]) ++result;
            }
        }
    }
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int d; cin >> d;
    while (d--) {
        do_testcase();
        if (d) memset(a, 0, sizeof a);
    }
}
