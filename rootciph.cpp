// 2014-04-26
#include <iostream>
#include <string>
using namespace std;
int main() {
    int T; cin >> T;
    while (T--) {
        long long a, b; string s; cin >> a >> b >> s;
        cout << a*a - 2*b << endl;
    }
}
