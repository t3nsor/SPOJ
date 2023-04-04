// 2023-04-04
#include <stdio.h>
#include <vector>
using namespace std;
int main() {
    // generate all primes
    vector<int> primes;
    for (int i = 2; i < 46340; i++) {
        bool is_prime = true;
        for (int j = 3; j * j <= i; j++) {
            if (i % j == 0) {
               is_prime = false;
               break;
            }
        }
        if (is_prime) primes.push_back(i);
    }
    for (;;) {
        int a, b; scanf("%d%d", &a, &b); if (!a) break;
        // find all factors of [a ... b]
        vector<vector<int>> factors(b - a + 1);
        for (int i = 0; i < primes.size() && primes[i] * primes[i] <= b; i++) {
            for (int j = (a + primes[i] - 1) / primes[i];
                 j <= b / primes[i];
                 j++) {
                factors[j * primes[i] - a].push_back(primes[i]);
            }
        }
        int cnt = 0;
        for (unsigned int i = a; i <= b; i++) {
            int j = i;
            int nf = factors[i - a].size();
            if (i % 2 == 0) continue;
            bool fail = false;
            for (int q : factors[i - a]) {
                if (j % (q * q) == 0 || (i - 1) % (q - 1)) {
                    fail = true;
                    break;
                }
                j /= q;
            }
            if (fail) continue;
            if (j > 1) {
                ++nf;
                if ((i - 1) % (j - 1)) continue;
            }
            if (nf >= 3) {
                ++cnt;
                printf("%d\n", i);
            }
        }
        if (!cnt) puts("none");
    }
}
