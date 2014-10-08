// 2014-09-22
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
int gcd(int x, int y) {
    if (x == 0) return y;
    else return gcd(y%x, x);
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        string s; cin >> s;
        int p = s.find(".");
        if (p == string::npos) {
            cout << "1\n";
        } else {
            string f = s.substr(p+1);
            while (f.size() < 4) f += "0";
            istringstream iss(f);
            int frac; iss >> frac;
            cout << 10000/gcd(10000, frac) << "\n";
        }
    }
    return 0;
}
