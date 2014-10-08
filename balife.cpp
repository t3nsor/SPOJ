// 2014-05-03
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <algorithm>
using namespace std;
int main() {
    for (;;) {
        int N; scanf("%d", &N); if (N == -1) return 0;
        int sum = 0;
        vector<int> a;
        for (int i = 0; i < N; i++) {
            int x; scanf("%d", &x); a.push_back(x);
            sum += x;
        }
        if (sum % N != 0) {
            puts("-1"); continue;
        }
        for (int i = 0; i < N; i++) {
            a[i] -= sum/N;
        }
        int res = 0;
        sum = 0;
        for (int i = 0; i < N; i++) {
            sum += a[i];
            res = max(res, abs(sum));
        }
        printf("%d\n", res);
    }
    return 0;
}
