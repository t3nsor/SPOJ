// 2025-05-31
// This C++ solution was checked against `hardp.py`, which was my original
// solution and contains the explanation. For some reason Python is really slow
// on the SPOJ grader and gets TLE even though it takes something like 0.02s on
// my computer. Using the Python solution it's possible to discover that all the
// coefficient numerators and denominators fit in a 128-bit integer, which makes
// it less painful than expected to rewrite the algorithm in C++.

#include <iostream>
#include <utility>
#include <vector>
using namespace std;

unsigned __int128 gcd(unsigned __int128 x, unsigned __int128 y) {
    if (x == 0) return y;
    return gcd(y % x, x);
}

unsigned __int128 lcm(unsigned __int128 x, unsigned __int128 y) {
    const auto g = gcd(x, y);
    return (x/g) * y;
}

struct Frac {
    __int128 num;
    unsigned __int128 den;

    void mul(int n) {
        bool sign = (num < 0);
        if (sign) num = -num;
        if (n < 0) {
            sign = !sign;
            n = -n;
        }
        const auto g = gcd(n, den);
        den /= g;
        num *= (n / g);
        if (sign) num = -num;
    }

    void div(int n) {
        bool sign = (num < 0);
        if (sign) num = -num;
        if (n < 0) {
            sign = !sign;
            n = -n;
        }
        const auto g = gcd(n, num);
        num /= g;
        den *= (n / g);
        if (sign) num = -num;
    }
};

void print(__int128 x) {
    if (x < 0) {
        putchar('-');
        x = -x;
    }
    static char buf[50];
    buf[0] = '0' + (x % 10);
    x /= 10;
    int len = 1;
    while (x > 0) {
        buf[len++] = '0' + (x % 10);
        x /= 10;
    }
    while (len--) {
        putchar(buf[len]);
    }
}

void printplus(__int128 x) {
    if (x >= 0) putchar('+');
    print(x);
}

vector<Frac> S[51];
unsigned __int128 den[51];
vector<__int128> Si[51];
int main() {
    S[0].push_back({0, 1});
    S[0].push_back({1, 1});
    for (int k = 1; k <= 50; k++) {
        vector<Frac> Sk = {{0, 1}, {0, 1}};
        for (int i = 1; i <= k; i++) {
            Frac a = S[k - 1][i];
            a.mul(k);
            a.div(i + 1);
            Sk.push_back(a);
        }
        S[k] = move(Sk);
        den[k] = 1;
        for (int i = 2; i <= k + 1; i++) {
            den[k] = lcm(den[k], S[k][i].den);
        }
        __int128 Si1 = den[k];
        for (auto a : S[k]) {
            a.mul(den[k]);
            Si[k].push_back(a.num);
            Si1 -= Si[k].back();
        }
        Si[k][1] = Si1;
        S[k][1].num = Si1;
        S[k][1].den = 1;
        S[k][1].div(den[k]);
    }
    for (;;) {
        int N; cin >> N; if (cin.eof()) return 0;
        printf("%d=", N);
        print(den[N]);
        printf("*(0");
        for (int i = 2; i <= N + 1; i += 2) {
            printplus(Si[N][i]);
        }
        for (int i = 1; i <= N + 1; i += 2) {
            printplus(Si[N][i]);
        }
        puts(")");
    }
}
