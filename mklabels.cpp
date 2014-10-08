// 2014-05-01
#include <cstdio>
using namespace std;
int f(int n) {
    int res = 1;
    for (int i = 0; i < n-2; i++) {
        res *= n;
    }
    return res;
}
int main() {
    for (int cs = 1; ; cs++) {
        int n; scanf("%d", &n);
        if (n == 0) return 0;
        printf("Case %d, N = %d, # of different labelings = %d\n", cs, n,
               f(n));
    }
}
