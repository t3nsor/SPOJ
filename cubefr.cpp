// 2014-05-01
#include <cstdio>
#include <cstring>
using namespace std;
const int primes[25] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43,
47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
bool cubefree[1000001];
int rank[1000001];
int main() {
    memset(cubefree, 1, sizeof cubefree);
    for (int i = 0; i < 25; i++) {
        int cube = primes[i]*primes[i]*primes[i];
        for (int j = cube; j <= 1000000; j += cube) {
            cubefree[j] = false;
        }
    }
    int sum = 0;
    for (int i = 1; i <= 1000000; i++) {
        if (cubefree[i]) {
            rank[i] = ++sum;
        }
    }
    int T; scanf("%d", &T);
    for (int cs = 1; cs <= T; cs++) {
        int n; scanf("%d", &n);
        printf("Case %d: ", cs);
        if (cubefree[n]) {
            printf("%d\n", rank[n]);
        } else {
            printf("Not Cube Free\n");
        }
    }
}
