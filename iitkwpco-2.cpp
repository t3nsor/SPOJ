// 2014-10-07
#include <cstdio>
#include <algorithm>
using namespace std;
int a[100];
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        int n; scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%d", a + i);
        }
        sort(a, a+n);
        int res = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (a[j] == 2*a[i]) {
                    a[j] = -1;
                    res++;
                    break;
                }
            }
        }
        printf("%d\n", res);
    }
}
