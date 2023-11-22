// 2023-11-21
#include <iostream>
using namespace std;
using ll = long long;
ll pow10[15];
void advance(int* digits, int e, int& sign, ll& result) {
    // count number of digits
    int len = 0;
    for (int i = 0; i < 16; i++) {
        if (digits[i] > 0) {
            len = i + 1;
        }
    }
    if (len % 2 == 0) {
        if (e % 2 == 0) {
            // All contributions from the least significant `e` digits cancel.
            ll s = 0;
            for (int i = len - 1; i >= e; i--) {
                s += sign * digits[i];
                sign *= -1;
            }
            result += pow10[e] * s;
        } else {
            // All contributions from the least significant `e` digits cancel,
            // except for one, which has the opposite sign to `sign`
            ll s = 0;
            for (int i = len - 1; i >= e; i--) {
                s += sign * digits[i];
                sign *= -1;
            }
            result += pow10[e] * s;
            result += sign * 45 * pow10[e - 1];
            sign *= -1;
        }
    } else {
        if (e == 0) {
            for (int i = len - 1; i >= e; i--) {
                result += sign * digits[i];
                sign *= -1;
            }
        } else {
            // Contributions from the least significant `e` digits cancel except
            // for the ones' digit, which goes like 0-1+2-... The contribution
            // from the leading digits also cancel.
            result -= sign * pow10[e] / 2;
        }
    }
    // increment the number
    for (;;) {
        digits[e]++;
        if (digits[e] == 10) {
            digits[e] = 0;
            e++;
        } else {
            return;
        }
    }
}
ll do_testcase(ll n) {
    ++n;
    int ndigits[16];
    for (int i = 0; i < 16; i++) {
        ndigits[i] = (n / pow10[i]) % 10;
    }
    int curdigits[16] = {1};
    ll result = 0;
    int sign = 1;
    for (;;) {
        // find most significant place where `curdigits` and `ndigits` differ
        int e = -1;
        for (int i = 15; i >= 0; i--) {
            if (ndigits[i] != curdigits[i]) {
                e = i;
                break;
            }
        }
        if (e == -1) return result;
        // but if anything below that isn't a 0, then advance it first
        for (int i = 0; i < e; i++) {
            if (curdigits[i] != 0) {
                e = i;
                break;
            }
        }
        advance(curdigits, e, sign, result);
    }
}
int main() {
    ll p = 1;
    for (int i = 0; i < 16; i++) {
        pow10[i] = p;
        p *= 10;
    }
    for (;;) {
        ll N; cin >> N;
        if (N == 0) return 0;
        cout << do_testcase(N) << '\n';
    }
}
