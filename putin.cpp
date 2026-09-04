// 2026-09-03
// Needs Clang: the GCC version on SPOJ is too old to have the add with carry
// and subtract with borrow builtins.  It gets AC with GCC without the builtins,
// but takes 9.99s (very close to TLE).
#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
using namespace std;
using ULL = unsigned long long;
struct Bit320 {
    array<ULL, 5> a;

    Bit320(ULL x = 0) : a{x, 0, 0, 0, 0} {}

    friend Bit320 operator+(const Bit320& lhs, const Bit320& rhs) {
        Bit320 result;
        ULL carry = 0;
        for (int i = 0; i < 5; i++) {
            result.a[i] = __builtin_addcll(lhs.a[i], rhs.a[i], carry, &carry);
        }
        return result;
    }

    friend Bit320 operator-(const Bit320& lhs, const Bit320& rhs) {
        Bit320 result;
        ULL carry = 0;
        for (int i = 0; i < 5; i++) {
            result.a[i] = __builtin_subcll(lhs.a[i], rhs.a[i], carry, &carry);
        }
        return result;
    }
};

void mul2(string& x) {
    int carry = 0;
    for (int i = 0; i < x.size(); i++) {
        x[i] = 2*x[i] + carry;
        if (x[i] >= 10) {
            x[i] -= 10;
            carry = 1;
        } else carry = 0;
    }
    if (carry == 1) x.push_back(1);
}

// this is only ever called for even `x` so it never increases the length
void add1(string& x) {
    for (int i = 0; i < x.size(); i++) {
        x[i]++;
        if (x[i] == 10) x[i] = 0; else break;
    }
}

void print(const Bit320& x) {
    // we can do this the dumb slow way since it's an uncommon operation
    string result(1, 0);
    for (int i = 4; i >= 0; i--) {
        for (int j = 63; j >= 0; j--) {
            mul2(result);
            if (x.a[i] & (ULL(1) << j)) add1(result);
        }
    }
    for (char& c : result) c += '0';
    reverse(result.begin(), result.end());
    cout << result;
}

Bit320 comb[10031][31];

void do_testcase() {
    int D, S, N; cin >> D >> S >> N;
    int p[20][30];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < D; j++) cin >> p[i][j];
    }
    Bit320 result = comb[S + D][D];
    int bound[30];
    for (int i = 1; i < (1 << N); i++) {
        const int pc = __builtin_popcount(i);
        fill(bound, bound + D, 0);
        int i2 = i;
        while (i2 > 0) {
            const int pos = __builtin_ctz(i2);
            i2 -= (1 << pos);
            for (int j = 0; j < D; j++) bound[j] = max(bound[j], p[pos][j]);
        }
        int top = S + D;
        for (int j = 0; j < D; j++) top -= bound[j];
        if (D <= top) {
            Bit320 term = comb[top][D];
            if (pc % 2) result = result - term; else result = result + term;
        }
    }
    result = result - (1 + D*S);
    print(result);
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    for (int n = 0; n <= 10030; n++) {
        comb[n][0] = 1;
        for (int k = 1; k <= min(n - 1, 30); k++) {
            comb[n][k] = comb[n - 1][k - 1] + comb[n - 1][k];
        }
        if (n <= 30) comb[n][n] = 1;
    }

    int T; cin >> T; while (T--) do_testcase();
}
