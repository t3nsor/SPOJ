// 2025-05-21
// It's well known that there are n^{n-2} labelled unrooted trees with n
// vertices. That means there are n^{n-1} labelled rooted trees. Counting
// unlabelled trees is much harder, but according to the official solutions,
// contestants were allowed to use the Internet during the contest. Queries of
// type 3 are OEIS A000081. Queries of type 4 are OEIS A000055.
#include <iostream>
#include <vector>
using namespace std;
vector<int> divisors[1001];
int modexp(int b, int e, int m) {
    if (e == 0) return 1;
    int result = modexp(b, e / 2, m);
    result = (result * result) % m;
    if (e % 2) {
        result = (result * b) % m;
    }
    return result;
}
int modinv(int a, int p) {
    return modexp(a, p - 2, p);
}
int do_query34(int kind, int n, int p) {
    vector<int> a(n + 1);
    vector<int> dsum(n + 1);
    a[0] = 0;
    a[1] = 1;
    dsum[1] = 1;
    for (int i = 1; i + 1 <= n; i++) {
        for (int k = 1; k <= i; k++) {
            a[i + 1] = (a[i + 1] + dsum[k] * a[i - k + 1]) % p;
        }
        a[i + 1] = (a[i + 1] * modinv(i, p)) % p;
        for (const int d : divisors[i + 1]) {
            dsum[i + 1] = (dsum[i + 1] + d * a[d]) % p;
        }
    }
    if (kind == 3) return a[n];
    if (n <= 2) return 1;
    const int inv2 = modinv(2, p);
    int result = a[n];
    if (n % 2 == 0) result = (result + inv2 * a[n / 2]) % p;
    for (int i = 0; i <= n; i++) {
        const int term = (a[i] * a[n - i]) % p;
        result = (result + inv2 * (p - term)) % p;
    }
    return result;
}
int main() {
    for (int i = 1; i <= 1000; i++) {
        for (int j = 1; j <= i; j++) {
            if (i % j == 0) divisors[i].push_back(j);
        }
    }
    for (;;) {
        int kind, n, p; cin >> kind >> n >> p;
        if (cin.eof()) return 0;
        if (kind == 1) {
            if (n == 1) cout << "1\n";
            else cout << modexp(n % p, n - 2, p) << '\n';
        } else if (kind == 2) {
            cout << modexp(n % p, n - 1, p) << '\n';
        } else {
            cout << do_query34(kind, n, p) << '\n';
        }
    }
}
