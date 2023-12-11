// 2023-12-10
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;
void do_testcase() {
    int N; scanf("%d", &N);
    vector<int> id(N);
    vector<int> len(N);
    for (int i = 0; i < N; i++) {
        id[i] = i;
        len[i] = 0;
    }
    for (;;) {
        char c;
        do {
            c = getchar();
        } while (c <= 32);
        if (c == 'O') break;
        if (c == 'E') {
            int result = 0;
            int i; scanf("%d", &i); i--;
            while (i != id[i]) {
                result += len[i] += len[id[i]];
                i = id[i] = id[id[i]];
            }
            printf("%d\n", result);
        } else {
            int i, j; scanf("%d %d", &i, &j); --i; --j;
            const int l = abs(i - j) % 1000;
            int dist = 0;
            while (j != id[j]) {
                dist += len[j] += len[id[j]];
                j = id[j] = id[id[j]];
            }
            len[i] = dist + l;
            id[i] = j;
        }
    }
}
int main() {
    int T; scanf("%d", &T);
    while (T--) do_testcase();
}
