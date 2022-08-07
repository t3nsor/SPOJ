// 2022-08-07
#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct Value {
    int b;  // number of bundles used
    int s;  // number of stops used
    int l;  // instruction that the last stop occurred just prior to
};
bool operator<(Value v1, Value v2) {
    if (v1.b != v2.b) return v1.b < v2.b;
    if (v1.s != v2.s) return v1.s < v2.s;
    return v1.l > v2.l;
}
template <typename T>
void update1(int i, Value& out, Value prev, const T& exist,
             int inst, int dep) {
    const bool need_stop = dep >= prev.l;
    if (exist[inst][0][0][0] ||
        exist[0][inst][0][0] ||
        exist[0][0][inst][0]) {
        // This instruction can be in a bundle with no embedded stop
        out = min(out,
                  {prev.b + 1,
                   prev.s + need_stop,
                   need_stop ? i : prev.l});
    }
    if (exist[inst][0][0][1] ||
        exist[inst][0][0][2] ||
        exist[0][inst][0][2]) {
        // This instruction can be in a bundle with a stop after it
        out = min(out,
                  {prev.b + 1,
                   prev.s + need_stop + 1,
                   i + 1});
    }
    if (exist[0][inst][0][1] ||
        exist[0][0][inst][1] ||
        exist[0][0][inst][2]) {
        // This instruction can be in a bundle with an embedded stop before it
        out = min(out,
                  {prev.b + 1,
                   prev.s + 1,
                   i});
    }
}
template <typename T>
void update2(int i, Value& out, Value prev, const T& exist,
             int inst1, int dep1, int inst2, int dep2) {
    const bool need_stop_inside = (dep2 == i);
    if (!need_stop_inside && (exist[inst1][inst2][0][0] ||
                              exist[inst1][0][inst2][0] ||
                              exist[0][inst1][inst2][0])) {
        // These two instructions can be in a bundle with no embedded stop.
        // We may have to insert a stop before the first instruction if either
        // of the two instructions requires it
        const bool need_stop_before = (dep1 >= prev.l || dep2 >= prev.l);
        out = min(out,
                  {prev.b + 1,
                   prev.s + need_stop_before,
                   need_stop_before ? i : prev.l});
    }
    if (exist[inst1][inst2][0][1] || exist[0][inst1][inst2][2] ||
        exist[inst1][0][inst2][1] || exist[inst1][0][inst2][2]) {
        // These two instructions can be in a bundle with a stop between them.
        // Only the first stop needs to be taken into account in determining
        // whether to insert an additional stop before the bundle.
        const bool need_stop_before = dep1 >= prev.l;
        out = min(out,
                  {prev.b + 1,
                   prev.s + need_stop_before + 1,
                   i + 1});
    }
    if (exist[inst1][inst2][0][2] && !need_stop_inside) {
        // These two instructions can be in a bundle with an embedded stop
        // after both of them. We may have to insert a stop before the first
        // instruction if either of the two instructions requires it.
        const bool need_stop_before = (dep1 >= prev.l || dep2 >= prev.l);
        out = min(out,
                  {prev.b + 1,
                   prev.s + need_stop_before + 1,
                   i + 2});
    }
    if (exist[0][inst1][inst2][1] && !need_stop_inside) {
        // These two instructions can be in a bundle with an embedded stop
        // before both of them.
        out = min(out,
                  {prev.b + 1,
                   prev.s + 1,
                   i});
    }
}
template <typename T>
void update3(int i, Value& out, Value prev, const T& exist,
             int inst1, int dep1, int inst2, int dep2, int inst3, int dep3) {
    if (exist[inst1][inst2][inst3][0]) {
        // If we use a bundle with no embedded stop, we must ensure that no
        // embedded stop is needed.
        const bool need_stop_inside = (dep2 == i || dep3 >= i);
        if (!need_stop_inside) {
            // Place a stop before the bundle if any of the three instructions
            // has a dependency that requires it.
            const bool need_stop_before = (dep1 >= prev.l ||
                                           dep2 >= prev.l ||
                                           dep3 >= prev.l);
            out = min(out,
                      {prev.b + 1,
                       prev.s + need_stop_before,
                       need_stop_before ? i : prev.l});
        }
    }
    if (exist[inst1][inst2][inst3][1]) {
        // There is an embedded stop between instructions 1 and 2. Ensure that
        // none is needed between 2 and 3.
        const bool ok = (dep3 != (i + 1));
        if (ok) {
            // We need to insert a stop before the bundle only if the first
            // instruction has a dependency that requires it.
            const bool need_stop_before = dep1 >= prev.l;
            out = min(out,
                      {prev.b + 1,
                       prev.s + need_stop_before + 1,
                       i + 1});
        }
    }
    if (exist[inst1][inst2][inst3][2]) {
        // There is an embedded stop between instructions 2 and 3. Ensure that
        // none is needed between 1 and 2.
        const bool ok = dep2 != i;
        if (ok) {
            // We need to insert a stop before the bundle if either of the first
            // two instructions has a dependency that requires it.
            const bool need_stop_before = (dep1 >= prev.l || dep2 >= prev.l);
            out = min(out,
                      {prev.b + 1,
                       prev.s + need_stop_before + 1,
                       i + 2});
        }
    }
}
void do_testcase() {
    int M, N; cin >> M >> N;
    vector<int> inst, dep;
    bool exist[11][11][11][3];
    memset(exist, 0, sizeof(exist));
    while (M--) {
        string tmpl; int stop;
        cin >> tmpl >> stop;
        const int i1 = tmpl[0] - 'A' + 1;
        const int i2 = tmpl[1] - 'A' + 1;
        const int i3 = tmpl[2] - 'A' + 1;
        exist[i1][i2][i3][stop] = true;
        exist[i1][i2][ 0][stop] = true;
        exist[i1][ 0][i3][stop] = true;
        exist[i1][ 0][ 0][stop] = true;
        exist[ 0][i2][i3][stop] = true;
        exist[ 0][i2][ 0][stop] = true;
        exist[ 0][ 0][i3][stop] = true;
        exist[ 0][ 0][ 0][stop] = true;
    }
    for (int i = 0; i < N; i++) {
        char c; int d;
        cin >> c >> d;
        inst.push_back(c - 'A' + 1);
        dep.push_back(d - 1);
    }
    vector<Value> dp(N + 1);
    dp[0] = {0, 0, 0};
    for (int i = 1; i <= N; i++) {
        dp[i].b = 1000000;
        update1(i - 1, dp[i], dp[i-1], exist,
                inst[i-1], dep[i-1]);
        if (i >= 2) {
            update2(i - 2, dp[i], dp[i-2], exist,
                    inst[i-2], dep[i-2], inst[i-1], dep[i-1]);
        }
        if (i >= 3) {
            update3(i - 3, dp[i], dp[i-3], exist,
                    inst[i-3], dep[i-3],
                    inst[i-2], dep[i-2],
                    inst[i-1], dep[i-1]);
        }
    }
    cout << dp[N].b << ' ' << dp[N].s << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) { do_testcase(); }
}
