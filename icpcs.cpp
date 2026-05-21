// 2026-05-21
#include <algorithm>
#include <limits.h>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
struct Team {
    int solved = 0;
    int penalty = 0;
    vector<pair<int, int>> problems;
};
void do_testcase(int T, int P) {
    vector<Team> teams;
    for (int i = 0; i < T; i++) {
        Team team;
        for (int j = 0; j < P; j++) {
            int attempts; scanf("%d/", &attempts);
            int c = getchar();
            int minutes = -1;
            if (c != '-') {
                ungetc(c, stdin);
                scanf("%d", &minutes);
            }
            if (minutes < 0) attempts = 0;
            team.problems.emplace_back(attempts, minutes);
            if (minutes >= 0) {
                ++team.solved;
                team.penalty += minutes + 20*attempts;
            }
        }
        teams.push_back(team);
    }
    sort(teams.begin(), teams.end(),
         [](const Team& t1, const Team& t2) {
             return t1.solved < t2.solved ||
                    (t1.solved == t2.solved && t1.penalty > t2.penalty);
         });
    int l = 1, u = INT_MAX;
    for (int i = 1; i < T; i++) {
        for (int j = 0; j < i; j++) {
            const auto& t1 = teams[i];
            const auto& t2 = teams[j];
            if (t1.solved != t2.solved) continue;
            // `t1` either outranks `t2` or is tied
            int a1 = 0;
            for (int k = 0; k < P; k++) {
                a1 += t1.problems[k].first;
            }
            int a2 = 0;
            for (int k = 0; k < P; k++) {
                a2 += t2.problems[k].first;
            }
            if (t1.penalty == t2.penalty) {
                if (a1 != a2) l = u = 20;
            } else {
                if (a1 > a2) {
                    u = min(u, 20 + (t2.penalty - t1.penalty - 1) / (a1 - a2));
                } else if (a2 > a1) {
                    l = max(l, 20 - (t2.penalty - t1.penalty - 1) / (a2 - a1));
                }
            }
        }
    }
    printf("%d ", l);
    if (u == INT_MAX) putchar('*'); else printf("%d", u);
    putchar('\n');
}
int main() {
    for (;;) {
        int T, P; scanf("%d %d", &T, &P); if (T + P == 0) break;
        do_testcase(T, P);
    }
}
