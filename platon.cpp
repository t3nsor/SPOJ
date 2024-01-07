// 2024-01-07
// If we unwind all the statements made by P and S, we see that:
//
// There are at least 2 (a, b) with ab = P, but only one of those pairs such
// that the following holds:
// * there are at least 2 (a', b') with a' + b' = a + b, and for all such
//   (a', b'), there are at least 2 (a'', b'') with a''b'' = a'b'.
//
// For all (a, b) with a + b = S, there are at least 2 (a', b') with a'b' = ab.
//
// There are at least 2 (a, b) with a + b = S, but only one of those such that
// * there are at least 2 (a', b') with a'b' = ab, and
// * (a, b) is the only (a', b') pair such that
//   * a'b' = ab,
//   * there are at least 2 (a'', b'') with a'' + b'' = a' + b', and for all
//     such (a'', b''), there are at least 2 (a''', b''') with
//     a'''b''' = a''b''.
//
// Not sure why this is a C++03 only problem... annoying.

#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;

bool ok(int l, int r, int P, int S) {
    int cnt1 = 0, cnt2 = 0;
    for (int a = l; a <= r; a++) {
        if (P % a) continue;
        const int b = P / a;
        if (b <= a || b > r) continue;
        ++cnt1;
        int cnt3 = 0;
        bool fail = false;
        for (int a1 = l; a1 <= r; a1++) {
            const int b1 = a + b - a1;
            if (b1 <= a1 || b1 > r) continue;
            ++cnt3;
            int cnt4 = 0;
            for (int a2 = l; a2 <= r; a2++) {
                if ((a1 * b1) % a2) continue;
                const int b2 = a1 * b1 / a2;
                if (b2 <= a2 || b2 > r) continue;
                ++cnt4;
                if (cnt4 == 2) break;
            }
            if (cnt4 < 2) {
                fail = true;
                break;
            }
        }
        if (cnt3 >= 2 && !fail) {
            ++cnt2;
            if (cnt2 > 1) {
                return false;
            }
        }
    }
    if (cnt1 < 2 || cnt2 < 1) return false;

    cnt1 = cnt2 = 0;
    for (int a = l; a <= r; a++) {
        const int b = S - a;
        if (b <= a || b > r) continue;
        ++cnt1;
        int cnt3 = 0, cnt4 = 0;
        for (int a1 = l; a1 <= r; a1++) {
            if (a * b % a1) continue;
            const int b1 = a * b / a1;
            if (b1 <= a1 || b1 > r) continue;
            cnt3++;
            int cnt5 = 0;
            bool fail = false;
            for (int a2 = l; a2 <= r; a2++) {
                const int b2 = a1 + b1 - a2;
                if (b2 <= a2 || b2 > r) continue;
                ++cnt5;
                int cnt6 = 0;
                for (int a3 = l; a3 <= r; a3++) {
                    if (a2 * b2 % a3) continue;
                    const int b3 = a2 * b2 / a3;
                    if (b3 <= a3 || b3 > r) continue;
                    ++cnt6;
                    if (cnt6 >= 2) break;
                }
                if (cnt6 < 2) {
                    fail = true;
                    break;
                }
            }
            if (cnt5 >= 2 && !fail) {
                ++cnt4;
            }
        }
        if (cnt3 < 2) return false;
        if (cnt4 == 1) {
            ++cnt2;
            if (cnt2 > 1) break;
        }
    }
    return cnt1 >= 2 && cnt2 == 1;
}

struct Compare {
    bool operator()(pair<int, int>& p1, pair<int, int>& p2) const {
        return p1.first + p1.second < p2.first + p2.second;
    }
};

void do_testcase() {
    int l, r; scanf("%d %d", &l, &r); l++; r--;
    if (r <= l) {
        puts("0");
        return;
    }
    vector<pair<int, int> > v;
    for (int a = l; a <= r; a++) {
        for (int b = a + 1; b <= r; b++) {
            if (ok(l, r, a*b, a + b)) {
                v.push_back(make_pair(a, b));
            }
        }
    }
    sort(v.begin(), v.end(), Compare());
    printf("%d\n", (int)v.size());
    for (int i = 0; i < v.size(); i++) {
        printf("%d %d\n", v[i].first, v[i].second);
    }
}

int main() {
    int t; scanf("%d", &t);
    for (int i = 1; i <= t; i++) {
        printf("case %d\n", i);
        do_testcase();
    }
}
