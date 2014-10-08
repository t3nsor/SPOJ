// 2014-04-25
#include <iostream>
using namespace std;
char s[2001];
int main() {
    for (int cs = 1;; cs++) {
        scanf("%s", s);
        if (s[0] == '-') return 0;
        int cnt = 0;
        int wt = 0; 
        for (char* p = s; *p; p++) {
            if (*p == '{') {
                wt++;
            } else {
                if (wt == 0) {
                    wt = 1; cnt++;
                } else {
                    wt--;
                }
            }
        }
        printf("%d. %d\n", cs, cnt + wt/2);
    }
}
