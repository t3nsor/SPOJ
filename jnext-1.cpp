// 2014-05-26
#include <cstdio>
#include <algorithm>
using namespace std;
int a[1000000];
int main() {
    int t; scanf("%d", &t);
    while (t--) {
        int n; scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%d", a+i);
        }
        if (next_permutation(a, a+n)) {
            for (int i = 0; i < n; i++) {
                putchar(a[i] + '0');
            }
        } else {
            printf("-1");
        }
        putchar('\n');
    }
}
