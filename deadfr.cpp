// 2024-09-07
#include <iostream>
#include <string>
using namespace std;
long long pow10[19];
long long gcd(long long x, long long y) {
    if (x == 0) return y; else return gcd(y % x, x);
}
void reduce(long long& x, long long& y) {
    const long long g = gcd(x, y);
    x /= g;
    y /= g;
}
void do_testcase(string line) {
    line = line.substr(2, line.size() - 5);
    long long bestnum, bestden = 2e18;
    for (int repeat = 1; repeat <= line.size(); repeat++) {
        const int nonrepeat = line.size() - repeat;
        auto num = (pow10[repeat] - 1) *
                   (nonrepeat == 0 ? 0 : stoll(line.substr(0, nonrepeat))) +
                   stoll(line.substr(nonrepeat));
        auto den = (pow10[repeat] - 1) * pow10[nonrepeat];
        reduce(num, den);
        if (den < bestden) { bestnum = num; bestden = den; }
    }
    cout << bestnum << '/' << bestden << '\n';
}
int main() {
    pow10[0] = 1;
    for (int i = 1; i <= 18; i++) pow10[i] = 10 * pow10[i - 1];
    string line;
    for (;;) {
        cin >> line;
        if (line == "0") break;
        do_testcase(line);
    }
}
