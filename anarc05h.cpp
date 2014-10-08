// 2014-05-28
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
long long memo[26][230];
long long rec(const string& s, int pos, int last) {
    if (pos == s.length()) return 1;
    long long& res = memo[pos][last];
    if (res == -1) {
        res = 0;
        int sum = 0;
        for (int i = pos; i < s.length(); i++) {
            sum += s[i] - '0';
            if (sum >= last) {
                res += rec(s, i+1, sum);
            }
        }
    }
    return res;
}
int main() {
    for (int _cs = 1;; _cs++) {
        string s; cin >> s;
        if (s == "bye") return 0;
        memset(memo, -1, sizeof memo);
        printf("%d. %lld\n", _cs, rec(s, 0, 0));
    }
}
