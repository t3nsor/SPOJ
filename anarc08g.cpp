// 2014-10-01
#include <cstdio>
#include <vector>
using namespace std;
int main() {
    for (int _cs = 1;; _cs++) {
        int N; scanf("%d", &N);
        if (N == 0) return 0;
        vector<int> bal(N);
        int before = 0, after = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int x; scanf("%d", &x);
                before += x;
                bal[i] += x; bal[j] -= x;
            }
        }
        for (int i = 0; i < N; i++) {
            if (bal[i] > 0) after += bal[i];
        }
        printf("%d. %d %d\n", _cs, before, after);
    }
}
