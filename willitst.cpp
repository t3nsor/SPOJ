// 2014-04-24
#include <iostream>
#include <set>
using namespace std;
int main() {
    long long x;
    set<long long> S;
    cin >> x;
    while (S.find(x) == S.end() && x > 1) {
        S.insert(x);
        if (x%2 == 0) {
            x /= 2;
        } else {
            x = 3*x+3;
        }
    }
    if (x == 1) { cout << "TAK" << endl; }
    else { cout << "NIE" << endl; }
}
