// 2014-05-01
#include <iostream>
#include <string>
using namespace std;
int main() {
    for (;;) {
        string s, t; cin >> s >> t; if (s=="*") return 0;
        string w;
        int cnt = 0;
        int last = 0;
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == 'B' ^ t[i] == 'B') {
                if (last == 0) {
                    cnt++;
                }
            }
            last = s[i] == 'B' ^ t[i] == 'B';
        }
        cout << cnt << endl;
    }
}
