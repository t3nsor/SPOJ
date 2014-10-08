// 2014-10-07
#include <cstdio>
#include <cstring>
char num[55555];
bool div(int d, char* n) {
    int rem = 0;
    while (*n) rem = (10*rem + *n++ - '0')%d;
    return rem == 0;
}
int main() {
    int T; scanf("%d\n", &T);
    while (T--) {
        scanf("%s", num);
        printf("%s %s\n",
               div(252, num) ? "Yes" : "No",
               div(525, num) ? "Yes" : "No");
    }
}
