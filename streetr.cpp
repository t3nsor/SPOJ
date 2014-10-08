// 2014-04-30
#include <cstdio>
#include <algorithm>
using namespace std;
int gcd(int x, int y) {
    return x == 0 ? y : gcd(y%x, x);
}
int a[100000];
int main() {
    int N; scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d", a+i);
    }
    sort(a, a+N);
    int g = 0;
    for (int i = 1; i < N; i++) {
        g = gcd(g, a[i] - a[i-1]); 
    }
    printf("%d\n", (a[N-1] - a[0])/g - N + 1);
    return 0;
}
