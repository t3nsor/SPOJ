// 2014-05-27
#include <iostream>
using namespace std;
int gcd(int x, int y) {
    if (x == 0) return y; else return gcd(y%x, x);
}
int main() {
    int T; cin >> T;
    while (T--) {
        int A, B; cin >> A >> B;
        int g = gcd(A, B);
        cout << B/g << ' ' << A/g << endl;
    }
}
