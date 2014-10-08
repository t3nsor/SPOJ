// 2014-10-02
#include <iostream>
using namespace std;
int main() {
    long long fact[14];
    fact[0] = 1;
    for (int i = 1; i <= 13; i++) {
        fact[i] = fact[i-1] * i;
    }
    int K, N;
    while (cin >> N >> K) {
        long long res = fact[N];
        while (K--) {
            int x; cin >> x;
            res /= fact[x];
        }
        cout << res << endl;
    }
    return 0;
}
