// 2024-09-02
// The only tricky aspect to this problem is how to handle backslashes when
// markup mode is off. If the character following the backslash is '*', neither
// is printed. If the character following the backslash is anything other than
// '*' (or if there isn't one), the backslash is printed and the following
// character is processed as if it weren't preceded by a slash.
#include <ctype.h>
#include <stdio.h>
int main() {
    bool markup = true;
    enum State {
        NORMAL,
        ESCAPE,
        INT,
        FRAC
    } state = NORMAL;
    for (;;) {
        int c = getchar();
        if (c == EOF) break;
repeat:
        if (state == NORMAL) {
            if (c != '\\') {
                putchar(c);
            } else {
                state = ESCAPE;
            }
        } else if (state == ESCAPE) {
            if (c == '*') {
                // recognized even if `markup` is false
                markup = !markup;
                state = NORMAL;
            } else if (markup) {
                if (c == 's') {
                    state = INT;
                } else {
                    state = NORMAL;
                    if (c != 'b' && c != 'i') putchar(c);
                }
            } else {
                putchar('\\');
                state = NORMAL;
                goto repeat;
            }
        } else if (state == INT) {
            if (c == '.') {
                state = FRAC;
            } else if (!isdigit(c)) {
                state = NORMAL;
                goto repeat;
            }
        } else {
            if (!isdigit(c)) {
                state = NORMAL;
                goto repeat;
            }
        }
    }
    if (!markup && state == ESCAPE) putchar('\\');
}
