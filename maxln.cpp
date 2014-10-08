// 2014-04-25
#include <cstdio>
#include <cmath>
using namespace std;
int main() {
    int T;
    scanf("%d", &T);
    for (int i = 1; i <= T; i++) {
        double r;
        scanf("%lf", &r);
        printf("Case %d: ", i);
        if (r >= 0.25) {
            printf("%.2f\n", 4*r*r + 0.25);
        } else {
            printf("%.2f\n", 2*r);
        }
    }
}
