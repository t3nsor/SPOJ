// 2014-04-25
#include <iostream>
using namespace std;
int a[10000], b[10000];
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        int m, n;
        cin >> m;
        if (m==0) return 0;
        for (int i = 0; i < m; i++) {
            cin >> a[i];
        }
        cin >> n;
        for (int i = 0; i < n; i++) {
            cin >> b[i];
        }
        int sum1 = 0, sum2 = 0;
        int i = 0, j = 0;
        int res = 0;
        while (i < m || j < n) {
            if (i == m) {
                while (j < n) { sum2 += b[j++]; }
            } else if (j == n) {
                while (i < m) { sum1 += a[i++]; }
            } else if (a[i] < b[j]) {
                sum1 += a[i++];
            } else if (a[i] > b[j]) {
                sum2 += b[j++];
            } else {
                // intersection point!
                res += max(sum1, sum2);
                sum1 = sum2 = a[i];
                i++; j++;
            }
        }
        res += max(sum1, sum2);
        printf("%d\n", res);
    }
}
