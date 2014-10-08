// 2014-09-22
#include <vector>
#include <algorithm>
#include <cstdio>
using namespace std;
bool is_knumber[1000001];
int knumber_count[1000001];
bool is_prime(int x) {
    if (x < 2) return false;
    for (int i = 2; i*i <= x; i++) {
        if (x%i == 0) return false;
    }
    return true;
}
int main() {
    is_knumber[2] = true;
    for (int i = 2; i <= 1000; i++) {
        if (is_prime(i)) {
            int pwr = i*i;
            int sod = 1 + i + i*i;
            while (pwr <= 1000000) {
                if (is_prime(sod)) {
                    is_knumber[pwr] = true;
                }
                pwr *= i;
                sod += pwr;
            }
        }
    }
    knumber_count[0] = 0;
    for (int i = 1; i <= 1000000; i++) {
        knumber_count[i] = knumber_count[i-1] + int(is_knumber[i]);
    }
    int T; scanf("%d", &T);
    while (T--) {
        int A, B; scanf("%d %d", &A, &B);
        printf("%d\n", knumber_count[B] - knumber_count[A-1]);
    }
    return 0;
}
