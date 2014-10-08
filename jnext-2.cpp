// 2014-05-27
// custom implementation of next permutation
#include <cstdio>
#include <algorithm>
using namespace std;
int a[1000000];
int main() {
    int t; scanf("%d", &t);
    while (t--) {
        int n; scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%d", a+i);
        }
        int r = n-2;
        while (r >= 0 && a[r] >= a[r+1]) r--;
        if (r == -1) { puts("-1"); continue; }
        reverse(a+r+1, a+n);
        int i = r+1; while (a[i] <= a[r]) i++;
        swap(a[i], a[r]);
        for (int i = 0; i < n; i++) putchar('0' + a[i]);
        putchar('\n');
    }
}
