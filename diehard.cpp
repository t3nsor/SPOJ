// 2014-05-02
#include <iostream>
#include <map>
#include <utility>
using namespace std;
// 0 = air
// 1 = water
// 2 = fire
const int dh[3] = {3, -5, -20};
const int da[3] = {2, -10, 5};
struct S {
    int h;
    int a;
    int loc;
    bool operator<(const S& s) const {
        return h < s.h || h == s.h && a < s.a || h == s.h && a == s.a && loc < s.loc;
    }
};
map<S, int> memo;
int maxtime(int h, int a, int loc) {
    h += dh[loc]; a += da[loc];
    if (h <= 0 || a <= 0) return 0;
    int& res = memo[(S){h, a, loc}];
    if (res) return res;
    res = 0;
    for (int newloc = 0; newloc < 3; newloc++) {
        if (newloc != loc) {
            res = max(res, 1 + maxtime(h, a, newloc));
        }
    }
    return res;
}
int main() {
    int t; cin >> t;
    while (t--) {
        int h, a; cin >> h >> a;
        cout << maxtime(h, a, 0) << endl;
    }
}
