// 2014-05-01
#include <iostream>
#include <string>
#include <set>
using namespace std;
int main() {
    int c; cin >> c;
    while (c--) {
        set<string> winners;
        set<string> losers;
        for (int i = 0; i < 16; i++) {
            string s, t; int x, y; cin >> s >> t >> x >> y;
            if (x < y) swap(s, t);
            winners.insert(s); losers.insert(t);
        }
        for (set<string>::iterator I = losers.begin(); I != losers.end(); I++) {
            winners.erase(*I);
        }
        cout << *winners.begin() << endl;
    }
    return 0;
}
