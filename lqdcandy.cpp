// 2014-05-27
#include <cstdio>
using namespace std;
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        long long N; scanf("%lld", &N);
        long long pwr = 1, exponent = 0;
        while (pwr < N) { pwr <<= 1; exponent++; }
        int cnt = 0; while ((N&1) == 0) { N >>= 1; cnt++; }
        printf("%lld %d\n", pwr, exponent-cnt);
    }
}
