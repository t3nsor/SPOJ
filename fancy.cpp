// 2014-10-07
#include <cstdio>
#include <cstring>
int main() {
    int t; scanf("%d", &t);
    while (t--) {
        char num[50];
        scanf("%s", num);
        int l = strlen(num);
        int res = 1;
        for (int i = 1; i < l; i++) {
            if (num[i] == num[i-1]) {
                res <<= 1;
            }
        }
        printf("%d\n", res);
    }
    return 0;
}
