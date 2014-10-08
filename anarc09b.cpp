// 2014-04-30
#include <iostream>
using namespace std;
int gcd(int x, int y) {
    return x == 0 ? y : gcd(y%x, x);
}
int main() {
    for (;;) {
        int w, h;
        cin >> w >> h; if (w == 0) return 0;
        long long lcm = (long long)w*h/gcd(w, h);
        cout << (lcm/w)*(lcm/h) << endl;
    }
}
