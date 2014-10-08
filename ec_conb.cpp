// 2014-06-06
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>
using namespace std;
string to_binary(int x) {
    string s;
    do {
        s += '0' + (x&1);
        x >>= 1;
    } while (x);
    reverse(s.begin(), s.end());
    return s;
}
int from_binary(string s) {
    int res = 0;
    for (int i = 0; i < s.length(); i++) {
        res <<= 1;
        res += s[i] - '0';
    }
    return res;
}
int main() {
    int N; scanf("%d", &N);
    while (N--) {
        int x; scanf("%d", &x);
        if (x&1) printf("%d\n", x);
        else {
            string s = to_binary(x);
            reverse(s.begin(), s.end());
            printf("%d\n", from_binary(s));
        }
    }
}
