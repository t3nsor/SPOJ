// 2014-05-01
#include <iostream>
using namespace std;
int main() {
    for (;;) {
        int N; cin >> N;
        if (N == 0) return 0;
        cout << (3*(N+1)*(N+1) - (N+1))/2 << endl;
    }
}
