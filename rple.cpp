// 2014-10-07
#include <cstdio>
#include <set>
using namespace std;
int main() {
    int T; scanf("%d", &T);
    for (int _cs = 1; _cs <= T; _cs++) {
        int N, R; scanf("%d %d", &N, &R);
        set<int> spies, targets;
        bool spied = false;
        while (R--) {
            int x, y; scanf("%d %d", &x, &y);
            spies.insert(x); targets.insert(y);
        }
        for (set<int>::iterator I = spies.begin(); I != spies.end(); I++) {
            if (targets.find(*I) != targets.end()) {
                spied = true;
            }
        }
        printf("Scenario #%d: %s\n\n", _cs, spied ? "spied" : "spying");
    }
    return 0;
}
