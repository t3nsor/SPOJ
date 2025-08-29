// 2025-08-29
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;
void do_testcase(int cs, string s) {
    int bestc = 100;
    int bestl = -1;
    for (int i = 0; i < s.size(); i++) {
        for (int j = i + 1; j <= s.size(); j++) {
            string core = s.substr(i, j - i);
            if (!equal(core.begin(), core.end(), core.rbegin())) continue;
            int lcore = j - i;
            int lbefore = i;
            int lafter = s.size() - j;
            int c = lbefore + lafter;
            int l = lcore + 2*max(lbefore, lafter);
            if (c < bestc || (c == bestc && l > bestl)) {
                bestc = c;
                bestl = l;
            }
        }
    }
    printf("Case %d, sequence = %s, cost = %d, length = %d\n",
           cs, s.c_str(), bestc, bestl);
}
int main() {
    for (int cs = 1;; cs++) {
        string s; cin >> s;
        if (s.empty()) break;
        do_testcase(cs, s);
    }
}
