// 2014-04-30
#include <iostream>
#include <cstdlib>
using namespace std;
int gcd(int x, int y) {
    return x == 0 ? y : gcd(y%x, x);
}
int main() {
    int t; cin >> t;
    while (t--) {
        int x, y; cin >> x >> y;
        int g = gcd(abs(x), abs(y));
        cout << abs(x/g-y/g) << endl;
    }
}
