// 2025-06-15
#include <stdio.h>
#include <string>
using namespace std;
void do_testcase(const string& s) {
    int i = 0;
    while (i < s.size()) {
        if (i + 1 < s.size() && s[i + 1] == s[i]) {
            int n = 2;
            while (n < 9 && i + n < s.size() && s[i + n] == s[i]) ++n;
            putchar('0' + n);
            putchar(s[i]);
            i += n;
        } else {
            int j = i + 1;
            while (j < s.size() && (j + 1 == s.size() || s[j] != s[j + 1])) {
                j++;
            }
            putchar('1');
            while (i < j) {
                char c = s[i++];
                putchar(c);
                if (c == '1') putchar(c);
            }
            putchar('1');
        }
    }
    putchar('\n');
}
int main() {
    for (;;) {
        string s;
        for (;;) {
            int c = getchar_unlocked();
            if (c == EOF) return 0;
            if (c == '\n') break;
            s.push_back(c);
        }
        do_testcase(s);
    }
}
