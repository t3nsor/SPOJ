// 2025-07-03
#include <stdio.h>
#include <string>
using namespace std;
int main() {
    string thue = "0";
    while (thue.length() < 5001) {
        int n = thue.length();
        for (int i = 0; i < n; i++) {
            thue.push_back(thue[i] == '0' ? '1' : '0');
        }
    }
    string thuediff;
    for (int i = 0; i < 5000; i++) {
        if (thue[i] < thue[i + 1]) {
            thuediff.push_back('N');
        } else if (thue[i] == thue[i + 1]) {
            thuediff.push_back('O');
        } else {
            thuediff.push_back('P');
        }
    }
    for (;;) {
        int n; scanf("%d", &n); if (n == 0) return 0;
        for (int i = 0; i < n; i++) putchar(thuediff[i]);
        putchar('\n');
    }
}
