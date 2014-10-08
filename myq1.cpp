// 2014-05-01
#include <cstdio>
using namespace std;
int main() {
    int t; scanf("%d", &t);
    while (t--) {
        int x; scanf("%d", &x);
        if (x == 1) {
            printf("poor conductor\n");
            continue;
        }
        int foo = (x-2)/10;
        int bar = (x-2)%10;
        int row;
        if (bar < 5) {
            row = 2*foo+1;
        } else {
            row = 2*foo+2;
            bar = 9 - bar;
        }
        printf("%d ", row);
        switch (bar) {
            case 0: printf("W L\n"); break;
            case 1: printf("A L\n"); break;
            case 2: printf("A R\n"); break;
            case 3: printf("M R\n"); break;
            case 4: printf("W R\n");
        }
    }
}
