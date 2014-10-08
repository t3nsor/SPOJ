// 2014-05-01
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
using namespace std;
int main() {
    for (;;) {
        string s = ""; cin >> s; if (s == "") return 0;
        int cnt = 0;
        for (int i = 0; i < s.length(); i++) {
            cnt += !!isupper(s[i]) ^ (i%2);
        }
        cout << min(cnt, (int)s.length() - cnt) << endl;
    }
}
