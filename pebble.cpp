// 2014-05-02
#include <iostream>
#include <string>
using namespace std;
int main() {
    for (int cs = 1;; cs++) {
        string s; cin >> s; s = "0" + s;
        if (cin.eof()) return 0;
        printf("Game #%d: ", cs);
        int cnt = 0;
        for (int i = 1; i < s.length(); i++) {
            cnt += (s[i] != s[i-1]);
        }
        printf("%d\n", cnt);
    }
}
