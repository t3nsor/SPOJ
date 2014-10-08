// 2014-05-02
#include <cstdio>
#include <algorithm>
using namespace std;
int main() {
    int a[4]; int cs = 0;
    while (~scanf("%d %d %d %d", a, a+1, a+2, a+3)) {
        sort(a, a+4);
        printf("Case %d: %d\n", ++cs, a[2] + a[3]);
    }
}
