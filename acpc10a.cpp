// 2014-04-24
#include <iostream>
#include <cstdio>
using namespace std;
int main() {
    long long x, y, z;
    for (;;) {
        scanf("%lld %lld %lld", &x, &y, &z);
        if (x == 0 && y == 0 && z == 0) return 0;
        if (z - y == y - x) {
            printf("AP %lld\n", 2*z - y);
        } else {
            if (y == 0) printf("GP 0\n");
            else printf("GP %lld\n", z*z/y);
        }
    }
}
