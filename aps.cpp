// 2014-09-26
#include <cstdio>
int minfact[10000000];
long long a[10000000];
int main() {
    for (int i = 2; i*i < 10000000; i++) {
        if (!minfact[i]) {
            // it's a prime
            for (int j = i; j < 10000000; j+=i) {
                if (!minfact[j]) minfact[j] = i;
            }
        }
    }
    for (int i = 2; i < 10000000; i++) {
        a[i] = a[i-1] + minfact[i];
        if (!minfact[i]) a[i] += i;
    }
    int T; scanf("%d", &T);
    while (T--) {
        int N; scanf("%d", &N); printf("%lld\n", a[N]);
    }
    return 0;
}
