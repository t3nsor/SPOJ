// 2026-06-22
// This is a classic annoying ACM-ICPC problem, not hard but annoying.  To avoid
// floating-point imprecision bugs, we use exclusively rational arithmetic.
// This is possible because the maximum speed for an "optimal scheme" is one of
// a finite set of rational speeds: either it's the maximum speed of 1/45 km/s,
// or the speed required to arrive at one of the ferries at the first crossing
// exactly when it leaves, or the speed required to disembark from one ferry and
// catch another ferry at the next crossing exactly when it leaves (we do need
// a way to avoid considering arbitrarily slow speeds, of course).
//
// Note that if the final section is a road, it is never optimal to travel at
// less than the speed limit, therefore the time required to complete a route
// is never a fractional number of seconds: the end of the scheme takes 45d
// seconds (where d is the total length of the roads at the end) and before
// that we either leave the origin on the hour, or disembark from the last
// ferry on the minute.
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
using namespace std;
using LL = long long;
LL gcd(LL x, LL y) { return y == 0 ? x : gcd(y, x % y); }
struct Rat {
    LL n;
    LL d;
    Rat(LL x) : n(x), d(1) {}
    Rat(LL n, LL d) : n(n), d(d) {
        if (d < 0) { n = -n; d = -d; }
        const LL g = gcd(abs(n), d);
        n /= g; d /= g;
    }
};
Rat operator/(const Rat& x, const Rat& y) {
    return Rat(x.n*y.d, y.n*x.d);
}
bool operator<(const Rat& x, const Rat& y) {
    return x.n*y.d < y.n*x.d;
}
bool operator==(const Rat& x, const Rat& y) {
    return x.n == y.n && x.d == y.d;
}
LL ceil(Rat r) {
    // I was lazy and only implemented this for nonnegative `n`
    if (r.n < 0) throw;
    return (r.n + r.d - 1) / r.d;
}
// return the minimum time (in seconds) to complete the route if our maximum
// speed is `speed`
int calc_time(const vector<vector<int>>& sections, Rat speed) {
    int cur = 0;
    for (const auto& section : sections) {
        if (section.size() == 1) {
            // this is a road
            cur += ceil(section[0]/speed);
        } else {
            // this is a ferry
            int rem = cur % 3600;
            int minwait = 1e6;
            for (int j = 1; j < section.size(); j++) {
                int wait = 60*section[j] - rem;
                if (wait < 0) wait += 3600;
                minwait = min(minwait, wait);
            }
            cur += minwait + 60*section[0];
        }
    }
    return cur;
}
pair<int, double> do_testcase(int s) {
    vector<vector<int>> sections;
    int roadlen = 0;  // total road length (km)
    int ftime = 0;  // total ferry time (seconds)
    for (int i = 0; i < s; i++) {
        string scratch1, scratch2; cin >> scratch1 >> scratch2;
        string kind; cin >> kind;
        vector<int> section;
        int x; cin >> x; section.push_back(x);
        if (kind == "ferry") {
            ftime += 60*x;
            int f; cin >> f;
            while (f--) {
                int d; cin >> d; section.push_back(d);
            }
            sections.push_back(section);
        } else {
            roadlen += x;
            // collapse adjacent roads
            if (!sections.empty() && sections.back().size() == 1) {
                sections.back()[0] += x;
            } else {
                sections.push_back(section);
            }
        }
    }
    s = sections.size();
    // determine minimum time to complete the route (in seconds)
    const int mintime = calc_time(sections, Rat(1, 45));
    // determine minimum speed that could possibly allow us to complete the
    // route in the minimum time
    const Rat minspeed = roadlen == 0
                         ? Rat(0)
                         : Rat(roadlen, mintime - ftime);
    vector<Rat> speeds = {minspeed, Rat(1, 45)};
    for (int i = 0; i < s - 1; i++) {
        if (sections[i].size() > 1) continue;
        // section `i` is a road, and section `i + 1` is a ferry
        const int l = sections[i][0];
        if (i == 0) {
            // candidate speeds are those allowing us to arrive at the first
            // ferry at exactly one of its departure times
            for (int j = 0;; j++) {
                const int h = j / (sections[i + 1].size() - 1);
                const int r = j % (sections[i + 1].size() - 1) + 1;
                const int t = 3600*h + 60*sections[i + 1][r];
                const Rat speed(l, t);
                if (speed < minspeed) break;
                if (speed < Rat(1, 45)) speeds.push_back(speed);
            }
        } else {
            // candidate speeds are those allowing us to arrive at the next
            // ferry at exactly one of its departure times after disembarking
            // from the previous ferry
            for (int j = 0; j < sections[i - 1].size() - 1; j++) {
                const int t1 = 60*(sections[i - 1][0] + sections[i - 1][j + 1]);
                for (int k = 0;; k++) {
                    const int h = k / (sections[i + 1].size() - 1);
                    const int r = k % (sections[i + 1].size() - 1) + 1;
                    const int t2 = 3600*h + 60*sections[i + 1][r];
                    if (t2 <= t1) continue;
                    const Rat speed(l, t2 - t1);
                    if (speed < minspeed) break;
                    if (speed < Rat(1, 45)) speeds.push_back(speed);
                }
            }
        }
    }
    sort(speeds.begin(), speeds.end());
    speeds.erase(unique(speeds.begin(), speeds.end()), speeds.end());
    int l = 0, r = speeds.size() - 1;
    while (r > l) {
        const int m = (l + r) / 2;
        if (calc_time(sections, speeds[m]) == mintime) {
            r = m;
        } else {
            l = m + 1;
        }
    }
    return {mintime, 3600*double(speeds[l].n) / speeds[l].d};
}
int main() {
    for (int cs = 1;; cs++) {
        int s; cin >> s;
        if (s == 0) break;
        const auto result = do_testcase(s);
        printf("Test Case %d: %02d:%02d:%02d %.2f\n\n",
               cs,
               result.first / 3600,
               (result.first / 60) % 60,
               result.first % 60,
               result.second);
    }
}
