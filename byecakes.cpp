// 2014-04-26
#include <iostream>
using namespace std;
int main() {
    for (;;) {
        int E, F, S, M, Ep, Fp, Sp, Mp;
        cin >> E >> F >> S >> M >> Ep >> Fp >> Sp >> Mp;
        if (E == -1) return 0;
        int n = 0;
        n = max(n, (E + (Ep - 1)) / Ep);
        n = max(n, (F + (Fp - 1)) / Fp);
        n = max(n, (S + (Sp - 1)) / Sp);
        n = max(n, (M + (Mp - 1)) / Mp);
        cout << Ep * n - E << " ";
        cout << Fp * n - F << " ";
        cout << Sp * n - S << " ";
        cout << Mp * n - M << endl;
    }
}
