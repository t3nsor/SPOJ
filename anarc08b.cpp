// 2014-05-01
#include <iostream>
#include <string>
#include <map>
#include <sstream>
using namespace std;
string codes[10] = {"063", "010", "093", "079", "106",
                    "103", "119", "011", "127", "107"};
map<string, int> digits;
int main() {
    ios::sync_with_stdio(false);
    for (int i = 0; i < 10; i++) {
        digits[codes[i]] = i;
    }
    for (;;) {
        string s; cin >> s;
        if (s == "BYE") return 0;
        int x = 0, i = 0;
        while (s[i] != '+') {
            x = 10*x + digits[s.substr(i, 3)];
            i += 3;
        }
        i++; int y = 0;
        while (s[i] != '=') {
            y = 10*y + digits[s.substr(i, 3)];
            i += 3;
        }
        cout << s;
        ostringstream oss; oss << x + y; string foo = oss.str();
        for (int i = 0; i < foo.length(); i++) {
            cout << codes[foo[i] - '0'];
        }
        cout << endl;
    }
}
