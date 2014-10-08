// 2014-10-07
#include <cstdio>
#include <vector>
using namespace std;
int main() {
    vector<long long> fib;
    fib.push_back(0);
    fib.push_back(1);
    for (;;) {
        long long x = fib[fib.size() - 2] + fib[fib.size() - 1];
        if (x < 1000000000) {
            fib.push_back(x);
        } else {
            break;
        }
    }
    int T; scanf("%d", &T);
    while (T--) {
        int n; scanf("%d", &n);
        printf("%lld\n", fib[n+2]);
    }
    return 0;
}
