// 2014-04-30
#include <iostream>
#include <algorithm>
using namespace std;
int a[1500];
int main() {
    for (;;) {
        int n; cin >> n; if (n == 0) return 0;
        for (int i = 0; i < n; i++)  {
            cin >> a[i];
        }
        a[n] = 0; a[n+1] = 1522;
        sort(a, a+n+2);
        bool ok = true;
        for (int i = 1; i < n+2; i++) {
            if (a[i] - a[i-1] > 200) {
                ok = false; break;
            }
        }
        cout << (ok ? "POSSIBLE" : "IMPOSSIBLE") << endl;
    }
}
