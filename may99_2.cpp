// 2014-10-07
#include <iostream>
#include <string>
using namespace std;
string letters = "manku";
string manku(long long n) {
    if (n == -1) return "";
    else return manku(n/5-1) + letters[n%5];
}
int main() {
    int t; cin >> t;
    while (t--) {
        long long n; cin >> n;
        cout << manku(n-1) << endl;
    }
    return 0;
}
