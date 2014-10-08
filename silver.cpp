// 2014-04-25
#include <iostream>
using namespace std;
int main() {
    for (;;) {
        int x;
        cin >> x; if (x == 0) return 0;
        int p = 1;
        int res = -1;
        while (p <= x) {
            p <<= 1;
            res++;
        }
        cout << res << endl;
    }
}
