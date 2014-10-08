// 2014-05-01
#include <cstdio>
using namespace std;
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        long long k; scanf("%*d %lld", &k); k--;
        int cnt = 0; while (k) { cnt += k&1; k>>=1; }
        if (cnt%2) { printf("Female\n"); }
        else { printf("Male\n"); }
    }
}
