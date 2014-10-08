// 2014-04-30
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
int main() {
    int T; cin >> T;
    while (T--) {
        string s; cin >> s;
        string r = s; reverse(r.begin(), r.end());
        cout << (s == r ? "YES" : "NO") << endl;
    }
}
