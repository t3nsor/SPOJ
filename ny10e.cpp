// 2014-05-01
#include <iostream>
using namespace std;
long long ncr[80][80];
int main() {
    for (int n = 0; n < 80; n++) {
        ncr[n][0] = 1;
    }
    for (int n = 1; n < 80; n++) {
        for (int k = 1; k < 80; k++) {
            ncr[n][k] = ncr[n-1][k] + ncr[n-1][k-1];
        }
    }
    int P; cin >> P;
    while (P--) {
        int cs, N; cin >> cs >> N;
        cout << cs << ' ' << ncr[N+9][N] << endl;
    }
}
