// 2014-04-25
#include <iostream>
using namespace std;
int main() {
    int T;
    cin >> T;
    while (T--) {
        int n, m, d;
        cin >> n >> m >> d;
        for (int i = 0; i < n; i++) {
            int x; cin >> x;
            m -= (x - 1) / d;
        }
        if (m > 0) {
            cout << "NO" << endl;
        } else {
            cout << "YES" << endl;
        }
    }
}
