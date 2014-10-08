// 2014-05-28
#include <cstdio>
#include <cmath>
using namespace std;
int main() {
    int t; scanf("%d", &t);
    while (t--) {
        double V; scanf("%lf", &V);
        double a = pow(4.0*V, 1.0/3.0);
        double h = 4.0*V/(a*a*sqrt(3.0));
        printf("%.2f\n", a*a*sqrt(3.0)/2.0 + 3.0*a*h);
    }
}
