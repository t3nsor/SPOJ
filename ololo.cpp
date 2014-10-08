// 2014-04-25
#include <cstdio>
int main() {
    int N;
    scanf("%d", &N);
    int res = 0;
    while (N--) {
        int x;
        scanf("%d", &x);
        res ^= x;
    }
    printf("%d\n", res);
}
