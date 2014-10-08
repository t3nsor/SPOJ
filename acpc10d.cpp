// 2014-04-27
#include <iostream>
using namespace std;
int main() {
    for (int cs = 1;; cs++) {
        int N; scanf("%d", &N);
        if (N == 0) return 0;
        int X, Y, Z, X_, Y_, Z_;
        int x, y, z;
        scanf("%d %d %d", &x, &y, &z);
        X = 2e9; Y = y; Z = y + z;
        for (int i = 1; i < N; i++) {
            scanf("%d %d %d", &x, &y, &z);
            X_ = x + min(X, Y);
            Y_ = y + min(min(X_, X), min(Y, Z));
            Z_ = z + min(min(Y_, Y), Z);
            X = X_; Y = Y_; Z = Z_;
        }
        printf("%d. %d\n", cs, Y);
    }
}
