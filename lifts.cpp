// 2023-12-17
// To simplify the math we work in units of half-metres and half-seconds. Lifts
// move at the speed of one half-metre per half-second, and the time at which
// two lifts meet might be at an odd number of half-seconds. At the end we
// simply divide by 2. The actual algorithm is just brute force because the
// numbers are small: for each pair of consecutive lifts, whose periods are P1
// and P2, respectively, we just simulate lcm(P1, P2) steps to see when they
// meet.
#include <iostream>
#include <vector>
using namespace std;
int gcd(int x, int y) {
    if (x == 0) return y;
    return gcd(y % x, x);
}
int do_testcase() {
    int n; cin >> n;
    vector<int> low(n + 2), high(n + 2), init(n + 2), initd(n + 2);
    // Pretend that the starting and ending edges of the chasm are also lifts
    // with zero range.
    low[0] = high[0] = init[0] = 0; initd[0] = 1;
    low[n+1] = high[n+1] = init[n+1] = 0; initd[n+1] = 1;
    for (int i = 1; i <= n; i++) {
        cin >> low[i] >> high[i] >> init[i] >> initd[i];
        low[i] *= 2; high[i] *= 2; init[i] *= 2;
    }
    int t = 2;
    for (int i = 0; i <= n; i++) {
        // Try to proceed from right end of this lift (which may be the left end
        // of the chasm) to left end of the next lift. To do this, we first need
        // to determine the position of the current & next lifts at the current
        // time (t).
        const int P1 = 2*(high[i] - low[i]);
        const int P2 = 2*(high[i+1] - low[i+1]);
        int y1 = init[i];
        int y2 = init[i+1];
        int d1 = initd[i];
        int d2 = initd[i+1];
        if (P1 > 0) {
            const int t1 = t % P1;
            for (int j = 0; j < t1; j++) {
                if (d1 == 1) {
                    if (y1 < high[i]) ++y1;
                    else { --y1; d1 = -1; }
                } else {
                    if (y1 > low[i]) --y1;
                    else { ++y1; d1 = 1; }
                }
            }
        }
        if (P2 > 0) {
            const int t2 = t % P2;
            for (int j = 0; j < t2; j++) {
                if (d2 == 1) {
                    if (y2 < high[i+1]) ++y2;
                    else { --y2; d2 = -1; }
                } else {
                    if (y2 > low[i+1]) --y2;
                    else { ++y2; d2 = 1; }
                }
            }
        }
        if (y1 == y2) goto end;
        if (P1 == 0 && P2 == 0) return -1;
        int lcm;
        if (P1 > 0 && P2 > 0) {
            lcm = P1 * P2 / gcd(P1, P2);
        } else {
            lcm = P1 + P2;
        }
        for (int j = 1; j < lcm; j++) {
            t++;
            if (P1 > 0) {
                if (d1 == 1) {
                    if (y1 < high[i]) ++y1;
                    else { --y1; d1 = -1; }
                } else {
                    if (y1 > low[i]) --y1;
                    else { ++y1; d1 = 1; }
                }
            }
            if (P2 > 0) {
                if (d2 == 1) {
                    if (y2 < high[i+1]) ++y2;
                    else { --y2; d2 = -1; }
                } else {
                    if (y2 > low[i+1]) --y2;
                    else { ++y2; d2 = 1; }
                }
            }
            if (y1 == y2) goto end;
        }
        return -1;
        // Move to the right edge of this lift, or to the final destination in
        // the case of the right end of the chasm.
        end:
        if (i < n) t += 4; else t += 2;
    }
    return t / 2;
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        cout << do_testcase() << '\n';
    }
}
