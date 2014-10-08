// 2014-04-30
#include <cstdio>
#include <algorithm>
using namespace std;
int main() {
    int T; scanf("%d", &T);
    for (int cs = 1; cs <= T; cs++) {
        int N; scanf("%d", &N);
        long long res = 1; long long sum = 0;
        for (int i = 0; i < N; i++)  {
            long long x; scanf("%lld", &x);
            sum += x; res = max(res, 1-sum);
        }
        printf("Scenario #%d: %lld\n", cs, res);
    }
}
