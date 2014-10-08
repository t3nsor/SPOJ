// 2014-04-30
#include <algorithm>
#include <cstdio>
#include <functional>
using namespace std;
long long a[100000];
long long b[100000];
int main() {
    for (;;) {
        int N; scanf("%d", &N); if (N == 0) return 0;
        for (int i = 0; i < N; i++) {
            scanf("%lld", a+i);
        }
        for (int i = 0; i < N; i++) {
            scanf("%lld", b+i);
        }
        sort(a, a+N);
        sort(b, b+N, greater<long long>());
        long long sum = 0;
        for (int i = 0; i < N; i++) {
            sum += a[i]*b[i];
        }
        printf("%lld\n", sum);
    }
}
