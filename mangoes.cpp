// 2014-09-30
#include <cstdio>
using namespace std;
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        long long N; scanf("%lld", &N);
        printf("%lld\n", ((N-1)/2)*((N-1)/2)%N);
    }
}
