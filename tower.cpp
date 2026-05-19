// 2026-05-19
#include <limits.h>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>
using namespace std;
// produces vector of (column, character) where the baseline is 0 and column
// number increases upward
void make_tower(vector<pair<int, char>>& result,
                unsigned long long x,
                unsigned long long b,
                const string& bs,
                int lvl) {
    unsigned long long divisor = 1;
    int exponent = 0;
    while (divisor <= ULLONG_MAX / b && b * divisor <= x) {
        divisor *= b;
        ++exponent;
    }
    bool first = true;
    for (; exponent >= 0; exponent--, divisor /= b) {
        unsigned long long coeff = x / divisor;
        x %= divisor;
        if (coeff == 0) continue;
        if (!first) {
            result.emplace_back(lvl, '+');
        }
        first = false;
        const string cs = to_string(coeff);
        if (exponent == 0) {
            for (const char c : cs) {
                result.emplace_back(lvl, c);
            }
        } else {
            if (coeff > 1) {
                for (const char c : to_string(coeff)) {
                    result.emplace_back(lvl, c);
                }
                result.emplace_back(lvl, '*');
            }
            for (const char c : bs) {
                result.emplace_back(lvl, c);
            }
            if (exponent > 1) make_tower(result, exponent, b, bs, lvl + 1);
        }
    }
}
void do_testcase(unsigned long long n, unsigned long long b) {
    const string bs = to_string(b);
    const string ns = to_string(n);
    vector<pair<int, char>> v;
    make_tower(v, n, b, bs, 0);
    int ht = 0;
    for (const auto& p : v) ht = max(ht, p.first + 1);
    int i = 0;
    const int ofs = ns.size() + 3;
    bool first = true;
    while (i < v.size()) {
        int j = i, k;
        while (j < v.size()) {
            k = j + 1;
            while (k < v.size() && v[k] != make_pair(0, '+')) k++;
            if (k - i <= 80 - ofs) j = k; else break;
        }
        putchar('\n');
        for (int l = ht - 1; l >= 0; l--) {
            int w = 0;
            for (int k = i; k < j; k++) {
                if (v[k].first == l) w = k - i + ofs + 1;
            }
            for (int k = 0; k < w; k++) {
                if (k < ofs) {
                    if (first && l == 0) {
                        if (k < ns.size()) {
                            putchar(ns[k]);
                        } else if (k == ns.size()) {
                            putchar(' ');
                        } else if (k == ns.size() + 1) {
                            putchar('=');
                        } else {
                            putchar(' ');
                        }
                    } else {
                        putchar(' ');
                    }
                } else if (v[k - ofs + i].first == l) {
                    putchar(v[k - ofs + i].second);
                } else {
                    putchar(' ');
                }
            }
            putchar('\n');
        }
        i = j;
        first = false;
    }
}
int main() {
    bool first = true;
    for (;;) {
        unsigned long long n, b; scanf("%llu %llu", &n, &b);
        if (n == 0 && b == 0) break;
        if (!first) {
            for (int i = 0; i < 80; i++) putchar('-');
            putchar('\n');
        }
        first = false;
        printf("%llu in complete base %llu:\n", n, b);
        do_testcase(n, b);
    }
}
