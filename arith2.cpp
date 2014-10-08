// 2014-04-24
#include <iostream>
#include <string>
using namespace std;
int main() {
    int T;
    cin >> T;
    while (T--) {
        long long res; cin >> res;
        string op;
        for (;;) {
            long long x;
            cin >> op;
            if (op == "+") {
                cin >> x; res += x;
            } else if (op == "-") {
                cin >> x; res -= x;
            } else if (op == "*") {
                cin >> x; res *= x;
            } else if (op == "/") {
                cin >> x; res /= x;
            } else {
                cout << res << endl;
                break;
            }
        }
    }
}
