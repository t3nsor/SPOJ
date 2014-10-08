// 2014-04-24
#include <iostream>
#include <algorithm>
using namespace std;
int main() {
    int x, y;
    for (;;) {
        cin >> x >> y;
        if (x == -1) return 0;
        if (x < y) swap(x, y);
        cout << (x + y)/(y + 1) << endl;
    }
}
