// 2014-04-30
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;
int main() {
    int N; cin >> N;
    while (N--) {
        long long k; cin >> k; k--;
        long long pwr = 2; int len = 1;
        while (k >= pwr) {
            k -= pwr; pwr <<= 1; len++; 
        }
        string res;
        while (len--) {
            if (k%2) {
                res.push_back('6');
            } else {
                res.push_back('5');
            }
            k >>= 1;
        }
        reverse(res.begin(), res.end());
        cout << res << endl;
    }
}
