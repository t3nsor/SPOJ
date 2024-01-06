// 2024-01-05
// The hardest part of this problem is understanding the problem statement. It's
// probably easiest to just explain the sample data: the first school gets the
// booklets with 3, 1, and 2 pages since those are the 3 shortest booklets. They
// have to be given in that order because that's the order in which they occur
// in the input. The second school gets the next 2 shortest booklets, in the
// order in which they occur in the input. The third school gets the remaining 2
// booklets, in the order in which they occur in the input. The numbers of
// booklets distributed to the schools are 3, 2, 2 because UIP (3) has to come
// before LIP (2). So it's straightforward to solve this problem by sorting. You
// can also do it in linear time, but whatever.
#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
void do_testcase() {
    int S, sidx, B; scanf("%d %d %d", &S, &sidx, &B);
    vector<pair<int, int>> v(B);
    for (int i = 0; i < B; i++) {
        v[i].second = i;
        scanf("%d", &v[i].first);
    }
    sort(v.begin(), v.end());
    const int UIP = (B + S - 1) / S;
    const int LIP = B / S;
    const int NUIP = B % S;
    int bbegin, bend;
    if (sidx <= NUIP) {
        bbegin = sidx * NUIP;
    } else {
        bbegin = NUIP * UIP + (sidx - NUIP) * LIP;
    }
    if (sidx < NUIP) {
        bend = (sidx + 1) * NUIP;
    } else {
        bend = NUIP * UIP + (sidx - NUIP + 1) * LIP;
    }
    pair<int, int> best(0, 10000);
    for (int i = bbegin; i < bend; i++) {
        if (v[i].second < best.second) {
            best = v[i];
        }
    }
    printf("%d\n", best.first);
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
