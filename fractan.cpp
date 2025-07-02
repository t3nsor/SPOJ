// 2025-07-01
// The trick with this one is not to overthink it. We obviously need to
// represent the current integer as its prime factorization. To simulate
// efficiently, when we loop over the fractions we need an efficient way to test
// whether the current integer is divisible by the fraction's denominator. I had
// an earlier solution using bitsets of size 193 that timed out (193 being the
// number of prime powers less than or equal to 1000). In fact, because each
// denominator is less than or equal to 1000, it can't have very many distinct
// prime factors, so doing the most naive thing is faster than the bitset
// approach.
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
bool is_composite[1000] = {0};
vector<pair<int, int>> factorize(int x) {
    vector<pair<int, int>> result;
    while (x > 1) {
        for (int p = 2;; p++) {
            if (x % p == 0) {
                pair<int, int> entry(p, 0);
                while (x % p == 0) {
                    ++entry.second;
                    x /= p;
                }
                result.push_back(entry);
                break;
            }
        }
    }
    return result;
}
void do_testcase(int m) {
    int N, k; cin >> N >> k;
    vector<vector<pair<int, int>>> num(k);
    vector<vector<pair<int, int>>> den(k);
    for (int i = 0; i < k; i++) {
        int a, b; cin >> a >> b;
        num[i] = factorize(a);
        den[i] = factorize(b);
    }
    int exponent[1000] = {0};
    const auto f = factorize(N);
    int nprimes = 0;  // number of *distinct* prime factors in current value
    for (const auto entry : f) {
        exponent[entry.first] = entry.second;
        nprimes++;
    }
    bool first = true;
    for (;;) {
        if (nprimes == 0 || (nprimes == 1 && exponent[2] > 0)) {
            if (!first) cout << ' ';
            first = false;
            cout << exponent[2];
            if (0 == --m) break;
        }
        int i = 0;
next:
        for (const auto entry : den[i]) {
            if (entry.second > exponent[entry.first]) {
                ++i;
                goto next;
            }
        }
        for (const auto entry : den[i]) {
            if (0 == (exponent[entry.first] -= entry.second)) --nprimes;
        }
        for (const auto entry : num[i]) {
            if (0 == exchange(exponent[entry.first],
                              exponent[entry.first] + entry.second)) ++nprimes;
        }
    }
    cout << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    for (int i = 2; i < 1000; i++) {
        if (is_composite[i]) continue;
        for (int j = i*i; j < 1000; j += i) {
            is_composite[j] = true;
        }
    }
    for (;;) {
        int m; cin >> m; if (m == 0) return 0;
        do_testcase(m);
    }
}
