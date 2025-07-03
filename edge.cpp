// 2025-07-03
#include <iostream>
#include <string>
#include <utility>
using namespace std;
void do_testcase(string s) {
    cout << "300 420 moveto\n310 420 lineto\n";
    int x = 310, y = 420;
    int dx = 10, dy = 0;
    for (const char c : s) {
        swap(dx, dy);
        if (c == 'V') {
            dx = -dx;
        } else {
            dy = -dy;
        }
        x += dx;
        y += dy;
        cout << x << ' ' << y << " lineto\n";
    }
    cout << "stroke\nshowpage\n";
}
int main() {
    for (;;) {
        string s; if (!(cin >> s)) return 0;
        do_testcase(s);
    }
}
