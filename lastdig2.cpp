// 2014-04-25
#include <iostream>
#include <string>
using namespace std;
int main() {
    int t; cin >> t;
    while (t--) {
        string a; long long b;
        cin >> a >> b;
        int last = a[a.length() - 1] - '0';
        if (b >= 8) {
            b = 4 + (b%4);
        }
        int res = 1;
        while (b--) {
            res = (res*last)%10;
        }
        cout << res << endl;
    }
}
