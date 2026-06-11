// 2014-04-30
#include <iostream>
#include <string>
using namespace std;
const int primes[8] = {2, 3, 5, 7, 11, 13, 17, 19};
int main() {
    for (;;) {
        string s; cin >> s; if (s == "*") return 0;
        long long res = 1;
        for (int i = 0; i < 8 && primes[i] <= s.length(); i++) { 
            for (int j = primes[i]; j <= s.length() && s[j-1] == 'Y'; j *= primes[i]) {
                res *= primes[i];
            }
        }
        bool ok = true;
        for (int i = 0; i < s.length(); i++) {
            if ((res % (i+1) == 0) ^ (s[i] == 'Y')) {
                ok = false;
            }
        }
        cout << (ok ? res : -1) << endl;
    }
}
