// 2014-05-27
#include <iostream>
#include <string>
using namespace std;
int main() {
    string s; cin >> s;
    int n = s.length();
    int a = 0, b = n, c = 0, d;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '1') a++;
    }
    d = n - a;
    for (int i = 1; i < s.length(); i++) {
        if (s[i] == '1' && s[i-1] == '0') c++;
    }
    c += (s[n-1] == '0' && s[0] == '1');
    // if a/b is greater, then shoot, otherwise rotate
    if (a*d > b*c) {
        printf("SHOOT\n");
    } else if (a*d == b*c) {
        printf("EQUAL\n");
    } else {
        printf("ROTATE\n");
    }
}
