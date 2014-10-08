// 2014-10-01
#include <cstdio>
int main() {
    bool occ[20002] = {0};
    int Q; scanf("%d", &Q);
    int tot = 0;
    while (Q--) {
        int p; scanf("%d", &p);
        tot++;
        tot -= occ[p-1];
        tot -= occ[p+1];
        occ[p] = true;
        printf("%d\n", tot);
    }
    puts("Justice");
    return 0;
}
