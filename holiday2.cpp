// 2023-10-20
// This problem is a lot easier than HOLIDAY1. We simply assume without proof
// that the set of all possible GCDs of subsets of the input is not too large,
// and compute this set explicitly. Then, we try every possible subset GCD to
// check whether it's achievable by removing at most k holidays (i.e., whether
// the periods that are divisible by that candidate GCD number at least n-k).
#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
using namespace std;
using ll = long long;
ll gcd(ll x, ll y) {
    return x == 0 ? y : gcd(y % x, x);
}
void do_testcase() {
    int n, k; cin >> n >> k;
    vector<ll> p(n);
    queue<ll> Q;
    set<ll> gcds;
    for (int i = 0; i < n; i++) {
        cin >> p[i];
        if (!gcds.count(p[i])) {
            Q.push(p[i]);
            gcds.insert(p[i]);
        }
    }
    while (!Q.empty()) {
        ll x = Q.front(); Q.pop();
        for (const ll y : p) {
            ll g = gcd(x, y);
            if (gcds.insert(g).second) {
                Q.push(g);
            }
        }
    }
    ll best = 0;
    for (const ll g : gcds) {
        if (count_if(p.begin(), p.end(), [g](ll x) { return x%g == 0; })
            >= n - k) {
            best = g;
        }
    }
    int keep = count_if(p.begin(), p.end(),
                        [best](ll x) { return x%best == 0; });
    int extra_remove = keep - (n - k);
    bool first = true;
    for (int i = 0; i < n; i++) {
        if (p[i] % best == 0) {
            if (extra_remove) {
                --extra_remove;
                if (!first) putchar(' ');
                first = false;
                printf("%d", i + 1);
            }
        } else {
            if (!first) putchar(' ');
            first = false;
            printf("%d", i + 1);
        }
    }
    putchar('\n');
}
int main() {
    int t; cin >> t;
    while (t--) do_testcase();
}
