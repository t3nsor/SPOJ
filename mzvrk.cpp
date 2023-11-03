// 2023-11-02
#include <iostream>
using namespace std;
// returns the sum of whirligigs of 1 ... x
unsigned long long compute(long long x) {
    unsigned long long result = 0;
    long long power = 1;
    for (;;) {
        // count the number of times that the whirligig is `power`
        long long cnt = x / power - x / (2 * power);
        if (cnt == 0) return result;
        result += cnt * power;
        power <<= 1;
    }
}
int main() {
    long long A, B; cin >> A >> B;
    cout << compute(B) - compute(A - 1) << '\n';
}
