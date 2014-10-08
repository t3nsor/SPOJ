// 2014-05-28
#include <iostream>
#include <cstdio>
using namespace std;
int sum_of_digits(int x) {
    if (x == 0) return 0; else return x%10 + sum_of_digits(x/10);
}
long long sum(int x) {
    // returns sum of digits in 0 + 1 + ... + (x-1)
    long long res = 0;
    long long cur = 0;
    long long increment = 1;
    int exponent = 0;
    while (10*increment <= x) { increment *= 10; exponent++; }
    while (cur < x) {
        if (cur + increment > x) {
            increment /= 10; exponent--; continue;
        }
        long long sod = sum_of_digits(cur);
        res += increment*sod + 45*exponent*increment/10;
        cur += increment;
    }
    return res;
}
int main() {
    for (;;) {
        int a, b; scanf("%d %d", &a, &b);
        if (a == -1) return 0;
        printf("%lld\n", sum(b+1) - sum(a));
    }
}
