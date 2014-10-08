// 2014-05-28
#include <iostream>
#include <map>
#include <set>
using namespace std;
int main() {
    int T; cin >> T;
    for (int _cs = 1; _cs <= T; _cs++) {
        int S; cin >> S;
        map<string, string> next;
        set<string> dests;
        for (int i = 0; i < S-1; i++) {
            string src, dest; cin >> src >> dest;
            next[src] = dest;
            dests.insert(dest);
        }
        map<string, string>::iterator I = next.begin();
        while (dests.find(I->first) != dests.end()) I++;
        printf("Scenario #%d:\n", _cs);
        string cur = I->first;
        for (int i = 0; i < S; i++) {
            cout << cur << endl;
            cur = next[cur];
        }
        cout << endl;
    }
}
