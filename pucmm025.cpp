// 2014-10-01
#include <cstdio>
#include <cstring>
using namespace std;
int main() {
    char s[250];
    while (~scanf("%s", s)) {
        int n = strlen(s);
        int rem[10];
        for (int i = 1; i <= 9; i++) {
            int m = 1;
            // constant optimization: construct mod tables
            int mod[100];
            for (int j = 0; j < 100; j++) {
                mod[j] = j%i;
            }
            rem[i] = 0;
            for (int j = n-1; j >= 0; j--) {
                rem[i] = mod[rem[i] + m*(s[j] - '0')];
                m = mod[m*10];
            }
        }
        int res = 0;
        for (int i = 0; i < n; i++) {
            if (s[i] != '0' && rem[s[i] - '0'] == 0) {
                res++;
            }
        }
        printf("%d\n", res);
    }
}
