// 2014-04-26
#include <iostream>
using namespace std;
int main() {
    int _t; cin >> _t;
    while (_t--) {
        int N, K, T, F;
        cin >> N >> K >> T >> F;
        cout << (F - N) * K / (K - 1) + N << endl;
    }
}
