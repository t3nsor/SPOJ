// 2014-05-01
#include <iostream>
#include <string>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) {
        int N; cin >> N;
        int cnt = 0;
        while (N--) {
            string s; cin >> s;
            if (s == "lxh") cnt++;
        }
        cout << (cnt % 2 ? "lxh" : "hhb") << endl;
    }
}
