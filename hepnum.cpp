// 2014-06-01
#include <cstdio>
#include <cstring>
using namespace std;
char s1[111111], s2[111111];
int main() {
    for (;;) {
        scanf("%s %s", s1, s2);
        if (s1[0] == '*') return 0;
        char *n1 = s1, *n2 = s2;
        while (n1[0] == '0') n1++;
        while (n2[0] == '0') n2++;
        int l1 = strlen(n1);
        int l2 = strlen(n2);
        if (l1 < l2) {
            puts("<"); continue;
        } else if (l1 > l2) {
            puts(">"); continue;
        }
        int res = strcmp(n1, n2);
        if (res < 0) {
            puts("<");
        } else if (res == 0) {
            puts("=");
        } else {
            puts(">");
        }
    }
}
