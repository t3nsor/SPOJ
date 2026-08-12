// 2026-08-12
// The hardest part of this problem is understanding the problem statement if
// you haven't played SC1 (which I haven't).  Important details include:
// * All probes can work in parallel during each time unit
// * Only one probe can be under construction at a time.
// * To construct a probe, you have to expend z units of minerals at the instant
//   when the construction begins (but the probe doesn't become operational
//   until 3 time steps later).
//
// When the problem statement has been understood, we see that it's essentially
// just a shortest path problem where we just need to do some basic calculations
// to limit the graph size:
// * Any state where you have more than 20 gas is equivalent to having 20 gas.
// * For minerals, a valid bound is 60.  If at time t you have 60 minerals, then
//   by time t + 10, you can have constructed 2 additional probes while still
//   having 20 minerals left over, and the probes you start with + the ones
//   you've constructed during this process will have produced at least 21 gas.
//   You wouldn't get a better solution by constructing 3 additional probes
//   after time t because it takes at least 9 seconds to construct them and thus
//   10 seconds for the 3rd additional probe to produce anything.  Since we need
//   to construct at most 2 probes once we have 60 minerals, there's no reason
//   to have more than 60.
// * Once you have 4 probes, it only takes 10 time units for them to collect 40
//   total gas and minerals, while it would also take 10 time units for it to be
//   worthwhile to construct 3 additional probes (since the last probe would
//   need at least 1 time unit to be useful) so you actually never need to
//   construct the 7th probe.  If you start with 5 to 7 probes, you never need
//   to construct more than 1 additional probe, while if you start with at least
//   8, then it never makes sense to construct probes.  So for any given test
//   case we would not consider states where the number of probes is more than
//   n+5.
// Counting the vertices and edges is left as an exercise for the reader, but
// the graph size is well within the bounds where BFS will pass; no need for A*
// or anything fancy.
#include <algorithm>
#include <queue>
#include <stdio.h>
#include <vector>
using namespace std;
int do_testcase() {
    int n, x, y, z, A, B; scanf("%d %d %d %d %d %d", &n, &x, &y, &z, &A, &B);
    int maxprobes;
    if (n <= 4) {
        maxprobes = 6;
    } else if (n <= 7) {
        maxprobes = n + 1;
    } else {
        maxprobes = n;
    }
    // lowest 6 bits: amount of minerals
    // next 5 bits: amount of gas
    // next 3 bits: number of probes minus n
    // highest 2 bits: time until next probe complete
    vector<int> dist(1 << 16, 1e9);
    queue<int> Q;
    dist[0] = 0;
    Q.push(0);
    const auto mkstate = [&](int m, int g, int p, int e) {
        m = min(m, 60);
        g = min(g, 20);
        return m + (g << 6) + ((p - n) << 11) + (e << 14);
    };
    for (;;) {
        const int state = Q.front();
        Q.pop();
        const int minerals = state & 63;
        const int gas = (state >> 6) & 31;
        const int probes = n + ((state >> 11) & 7);
        const int eta = state >> 14;
        for (int mprod = 0; mprod <= probes; mprod++) {
            int gprod = probes - mprod;
            const int m2 = minerals+x*mprod;
            const int g2 = gas+y*gprod;
            if (m2 >= A && g2 >= B) return dist[state] + 1;
            if (eta == 0 && probes < maxprobes && minerals >= z) {
                // consider constructing a probe
                const int s2 = mkstate(m2 - z, g2, probes, 2);
                if (dist[state] + 1 < dist[s2]) {
                    dist[s2] = dist[state] + 1;
                    Q.push(s2);
                }
            }
            // otherwise we can always not construct a probe
            int s2;
            if (eta == 0) {
                s2 = mkstate(m2, g2, probes, 0);
            } else if (eta == 1) {
                s2 = mkstate(m2, g2, probes + 1, 0);
            } else {
                s2 = mkstate(m2, g2, probes, 1);
            }
            if (dist[state] + 1 < dist[s2]) {
                dist[s2] = dist[state] + 1;
                Q.push(s2);
            }
        }
    }
}
int main() {
    int T; scanf("%d", &T); while (T--) printf("%d\n", do_testcase());
}
