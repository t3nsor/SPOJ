// 2014-04-30
#include <iostream>
#include <cmath>
#define pi acos(-1.0)
using namespace std;
int main() {
    int t; cin >> t;
    while (t--) {
        double x; cin >> x;
        if (x <= 1) cout << 1 << endl;
        else cout << (long long)(1 + floor((0.5*log(2.0*pi*x) + x*(log(x)-1))/log(10))) << endl;
    }
}
