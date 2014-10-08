// 2014-04-25
#include <iostream>
using namespace std;
int main() {
    int T;
    cin >> T;
    while (T--) {
        long long N;
        cin >> N;
        cout << (N*(N-1)/2 + N*(N+1))%1000007 << endl;
    }
}
