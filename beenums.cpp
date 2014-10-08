// 2014-04-25
#include <iostream>
#include <cmath>
using namespace std;
int main() {
    long long N;
    for (;;) {
        cin >> N;
        if (N == -1) return 0;
        long long disc = 9 + 12*(N-1);
        long long s = sqrt((double)disc);
        if (s*s == disc && (3 + s)%6 == 0 && (3+s)/6 >= 1) {
            cout << "Y" << endl;
        } else {
            cout << "N" << endl;
        }
    }
}
