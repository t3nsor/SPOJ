// 2024-09-20
// Observation 1: The restaurant makes exactly 2 full revolutions before closing.
// Observation 2: 360 degrees in 72 minutes = 1 centidegree in 12 centiseconds.
// A lot of simple-seeming approaches are wrong. The case where Christy's camera
// always recharges in time for the next landmark in angular order is easy, but
// the case where some landmarks need to be captured on the second revolution is
// tricky. Here, we use DP where the subproblem is a prefix of the sequence of
// landmarks (in angular order) and we remember what is the last landmark that we
// decided to photograph in the first revolution. We should never skip more than
// 2 landmarks in a row during the first revolution, because if we skipped 3 in a
// row, then there's no reason why the one in the middle couldn't have been done
// in the first revolution. We can skip 2 landmarks in a row only if they're far
// apart enough that both can be done in the second revolution. Note that the
// original test data (presumably used on SPOJ) are weak and you can get AC by
// only considering skipping 1 landmark at a time. The DMOJ version of this
// problem has additional test cases where skipping 2 is necessary.
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/// reads in a landmark and returns its position in centidegrees
int getPos() {
    string unused_name; cin >> unused_name;
    double degrees; cin >> degrees;
    return 100 * degrees + 0.5;
}
void do_testcase() {
    int n; cin >> n;
    vector<int> v;  // positions of landmarks, in centidegrees
    for (int i = 0; i < n; i++) {
        v.push_back(getPos());
    }
    sort(v.begin(), v.end());
    int r; cin >> r; r *= 100; // recharge time in centiseconds
    if (r == 0) throw;
    int best = 1e6;
    if (n > 2) {
        // check for impossible cases
        for (int i = 0; i < n; i++) {
            const int next = (i + 2) % n;
            const int arc = (v[next] - v[i] + 36000) % 36000;
            if (12 * arc < r) {
                cout << "not possible\n";
                return;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        // consider landmark `i` as Christy's starting point
        bool fail = false;
        int cur = 0;
        for (int j = (i + 1) % n; j != i; j = (j + 1) % n) {
            const int prev = (j + n - 1) % n;
            const int arc = (v[j] - v[prev] + 36000) % 36000;
            if (12 * arc < r) {
                fail = true;
                break;
            }
            cur += 12 * arc;
        }
        if (!fail) {
            cur += r;
            best = min(best, cur);
            continue;
        }
        // check whether we can do it within 2 revolutions
        vector<int> dp0(n, 1e6);  // last element used
        vector<int> dp1(n, 1e6);  // second last element used, last not
        vector<int> dp2(n, 1e6);  // third etc
        dp0[0] = 0;
        for (int j = 1; j < n; j++) {
            dp0[j] = min(dp1[j - 1], dp2[j - 1]);
            const int arc = (v[(i + j) % n] - v[(i + j - 1) % n] + 36000)
                            % 36000;
            const int arc2 = (v[(i + j) % n] - v[i] + 36000) % 36000;
            dp1[j] = max(dp0[j - 1], r + 12 * (36000 + arc2));
            if (12 * arc >= r) {
                dp0[j] = min(dp0[j], dp0[j - 1]);
                dp2[j] = max(dp1[j - 1], r + 12 * (36000 + arc2));
            }
        }
        best = min({best, dp0[n - 1], dp1[n - 1], dp2[n - 1]});
    }
    best = (best + 99) / 100;  // convert to seconds
    if (best > 8640) {
        cout << "not possible\n";
    } else {
        cout << best << '\n';
    }
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) do_testcase();
}
