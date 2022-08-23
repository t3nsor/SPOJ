// 2022-08-23
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
const long double log10_2 = log10l(2.0);
int digits_in_power(long double exponent) {
    return 1 + floor(log10_2 * exponent);
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        int a, b; cin >> a >> b;
        if (a == b) {
            cout << "7\n";
            continue;
        }
        vector<int> q;
        int sum = 0;
        while (a) {
            sum += (b / a) - 1;
            q.push_back(b / a);
            b %= a;
            swap(a, b);
        }
        const int l1 = digits_in_power(sum);
        // Note that `sum` being 0 is not possible unless we had a == b.
        const int l2 = digits_in_power(sum - 1);
        // To determine whether the first player has a winning strategy, note
        // that the first player always has a winning strategy if the first
        // entry in `q` is at least 2 (because they can decide whether to
        // exhaust this entry or to force the second player to do it). For
        // each initial entry with value 1, the outcome flips.
        size_t i = 0;
        while (i < q.size() && q[i] == 1) ++i;
        const int l3 = (i % 2 == 0 ? 5 : 6);
        cout << l1 + l2 + l3 << '\n';
    }
}
