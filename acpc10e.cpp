// 2014-10-07
#include <cstdio>
using namespace std;
int main() {
    for (;;) {
        long long G, T, A, D;
        scanf("%lld %lld %lld %lld", &G, &T, &A, &D);
        if (G == -1) return 0;
        printf("%lld*%lld/%lld+%lld=", G, A, T, D);
        long long stage1 = G*T*(T-1)/2;
        long long teams = G*A+D;
        long long newteams =
          teams == 1 ?
          1 :
          1ll << (64 - __builtin_clzll(teams - 1));
        printf("%lld+%lld\n", stage1 + newteams - 1, newteams - teams);
    }
}
