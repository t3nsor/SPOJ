// 2014-05-03
#include <cstdio>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <tr1/unordered_map>
using namespace std;
using namespace std::tr1;
int bit[30001];
int n;
int query(int x) {
    int res = 0;
    while (x > 0) {
        res += bit[x];
        x -= x & -x;
    }
    return res;
}
void update(int x, int inc) {
    while (x <= n) {
        bit[x] += inc;
        x += x & -x;
    }
}
char yoda[666666];
char normal[666666];
int main() {
    int t; scanf("%d", &t);
    while (t--) {
        scanf("%d\n", &n);
        memset(bit, 0, (n+1)*sizeof(int));
        gets(yoda);
        gets(normal);
        istringstream N((string(normal)));
        unordered_map<string, int> M;
        for (int i = 0; i < n; i++) {
            string s; N >> s; M[s] = i;
        }
        istringstream Y((string(yoda)));
        int res = 0;
        for (int i = 0; i < n; i++) {
            string s; Y >> s; int idx = M[s];
            res += i - query(idx);
            update(idx+1, 1);
        }
        printf("%d\n", res);
    }
}
