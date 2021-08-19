// 2020-08-19
//
// Idea: We have x^2 = a (mod 4000000007). According to FLT,
// x^4000000006 = 1 (mod 4000000007), and this implies x^2000000003 can only be
// congruent to +/- 1, therefore x^2000000004 is +/- x. But x^2000000004 is
// a^1000000002.
//
// Note that this only works for modulus of the form 4n+3.
#include <ctime>
#include <iostream>
using namespace std;
constexpr unsigned MODULUS = 4000000007;
unsigned pmod(unsigned base, int exponent) {
    int mask = 1;
    unsigned long long result = 1;
    unsigned long long p = base;
    while (exponent) {
        if (exponent & mask) {
            result = (result * p) % MODULUS;
            exponent &= ~mask;
        }
        p = (p * p) % MODULUS;
        mask <<= 1;
    }
    return result;
}
int main() {
    unsigned a;
    cin >> a;
    auto x = pmod(a, (MODULUS + 1) / 4);
    time_t t = x > MODULUS / 2 ? MODULUS - x : x;
    char result[27] = {};
    strftime(result, 27, "%c", gmtime(&t));
    cout << result << endl;
}
