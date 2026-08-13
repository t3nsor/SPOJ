// 2026-08-12
// I don't know how to prove it, but the optimal arrangements follow the below
// pattern for even n:
// 3 6 1 4
// 4 9 14 1 6 11
// 5 12 19 26 1 8 15 22
// ...
// That is, increasing by n-1 until the midpoint, and the second half is a
// shifted copy of the first half.
// For odd n:
// 3 7 11 1 5
// 4 16 9 21 1 13 6
// 5 29 20 11 35 1 25 16 7
// ...
// This pattern is a bit more complex: the second largest height is always in
// the second position, and we decrease by n-2 until just before the midpoint,
// before jumping back up to the max value at the midpoint, determined by taking
// the smallest value that wouldn't violate the constraints when compared with
// all values to its left.  Then the remainder is a shifted version of the first
// half again.
//
// How do we discover the above arrangements?  By brute force, obviously :)
#include <iostream>
#include <vector>
using namespace std;
vector<int> calc(int n) {
    // n = 3 is a special case that doesn't fit the odd general case below
    if (n == 3) {
        return {2, 4, 1};
    }
    vector<int> v;
    if (n % 2 == 0) {
        for (int i = 0; i < n / 2; i++) {
            v.push_back(i * (n - 1) + n / 2 + 1);
        }
        for (int i = 0; i < n / 2; i++) {
            v.push_back(i * (n - 1) + 1);
        }
    } else {
        v.push_back((n + 1) / 2);
        for (int i = 0; i < (n - 3) / 2; i++) {
            v.push_back(((n - 3)/ 2 - i)*n + 2);
        }
        v.push_back((n*n - 2*n + 7) / 2);
        for (int i = (n + 1) / 2; i < n; i++) {
            v.push_back(v[i - (n + 1) / 2] - (n - 1) / 2);
        }
    }
    return v;
}
int main() {
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        auto result = calc(n);
        for (int i = 0; i < n; i++) {
            printf("%d", result[i]);
            putchar(i == n - 1 ? '\n' : ' ');
        }
    }
}
