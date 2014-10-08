// 2014-04-25
#include <iostream>
#include <cstdio>
#include <map>
using namespace std;
int main() {
    int t; scanf("%d", &t);
    while (t--) {
        int n; scanf("%d", &n);
        map<int, int> m;
        for (int i = 0; i < n; i++) {
            int x; scanf("%d", &x); m[x]++;
        }
        bool found = false;
        for (map<int, int>::iterator I = m.begin(); I != m.end(); I++) {
            if (I->second > n/2) {
                found = true;
                printf("YES %d\n", I->first);
            }
        }
        if (!found) printf("NO\n");
    }
}
