// 2026-03-18
#include <map>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
char buf[4200000];
void do_testcase() {
    int R, C;
    scanf("%d", &C);
    fgets(buf, sizeof(buf), stdin);
    sscanf(buf, "%d", &R);
    const int len = R + C - 1;
    map<int, int> diff;
    for (int i = 0; i < R; i++) {
        fgets(buf, sizeof(buf), stdin);
        const char* s = buf;
        int col = 0;
        int rl, rv, numread;
        while (sscanf(s, "%d%d%n", &rl, &rv, &numread) > 0) {
            s += numread;
            diff[i + col] += rv;
            diff[i + col + rl] -= rv;
            col += rl;
        }
    }
    int curval = 0;
    int curidx = 0;
    vector<pair<int, int>> result;
    for (const auto& p : diff) {
        if (p.first > curidx) {
            result.emplace_back(p.first - curidx, curval);
        }
        curidx = p.first;
        curval += p.second;
    }
    if (curidx < R + C - 1) {
        result.emplace_back(R + C - 1 - curidx, curval);
    }
    vector<pair<int, int>> result2;
    for (const auto& p : result) {
        if (result2.empty()) {
            result2.push_back(p);
        } else if (result2.back().second == p.second) {
            result2.back().first += p.first;
        } else {
            result2.push_back(p);
        }
    }
    for (int i = 0; i < result2.size(); i++) {
        if (i > 0) putchar(' ');
        printf("%d %d", result2[i].first, result2[i].second);
    }
    putchar('\n');
}
int main() {
    int T; scanf("%d", &T); while (T--) do_testcase();
}
