// 2014-04-25
#include <iostream>
using namespace std;
int main() {
    int T; cin >> T;
    while (T--) {
        int R, C;
        cin >> R >> C;
        if (C >= R) {
            if (R % 2 == 0) {
                cout << "L";
            } else {
                cout << "R";
            }
        } else {
            if (C % 2 == 0) {
                cout << "U";
            } else {
                cout << "D";
            }
        }
        cout << endl;
    }
}
