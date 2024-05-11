// 2024-05-11
#include <stdio.h>
using namespace std;
int factor[1000001];
int main() {
    for (int i = 2; i <= 1000; i++) {
        if (factor[i] > 0) continue;
        for (int j = i; j <= 1000000; j += i) {
            factor[j] = i;
        }
    }
    int cnt = 0;
    for (int i = 6; i <= 1000000; i++) {
        if (i > 1000 && factor[i] == 0) continue;
        int nf = 0;
        int lastf = -1;
        int j = i;
        bool success = true;
        while (j > 1) {
            if (factor[j] == 0) factor[j] = j;
            if (factor[j] == lastf) {
                success = false;
                break;
            }
            lastf = factor[j];
            j /= lastf;
            ++nf;
        }
        if (success && nf >= 2) {
            ++cnt;
            if (cnt % 108 == 0) {
                printf("%d\n", i);
            }
        }
    }
}
