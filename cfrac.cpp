// 2024-09-05
// Although the problem statement implies that p can be up to 10^20 - 1, in
// reality it always fits in unsigned long long.
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;
unsigned long long gcd(unsigned long long a, unsigned long long b) {
    if (a == 0) return b; else return gcd(b % a, a);
}
void do_testcase(unsigned long long p, unsigned long long q) {
    vector<unsigned long long> v;
    for (;;) {
        const auto g = gcd(p, q);
        p /= g;
        q /= g;
        if (q == 1) break;
        v.push_back(p / q);
        p = exchange(q, p % q);
    }
    v.push_back(p - 1);
    int width = 3 * v.size() + 1;
    for (const auto x : v) {
        width += to_string(x).size();
    }
    vector<string> result(2 * v.size() + 1, string(width, '.'));
    int col = 0;
    for (int i = 0; i < v.size(); i++) {
        const int line = 2*i + 1;
        const auto s = to_string(v[i]);
        for (const char c : s) {
            result[line][col++] = c;
        }
        result[line][col++] = '.';
        result[line][col++] = '+';
        result[line][col++] = '.';
        for (int col2 = col; col2 < width; col2++) result[line][col2] = '-';
        const int onepos = (col + (width - 1)) / 2;
        result[line - 1][onepos] = '1';
    }
    result.back().back() = '1';
    for (const auto& l : result) cout << l << '\n';
}
int main() {
    unsigned long long p, q;
    int cs = 1;
    for (;;) {
        cin >> p >> q;
        if (p == 0) return 0;
        cout << "Case " << cs++ << ":\n" << p << " / " << q << '\n';
        do_testcase(p, q);
    }
}
