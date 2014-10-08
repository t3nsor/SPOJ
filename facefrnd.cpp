// 2014-04-25
#include <iostream>
#include <set>
using namespace std;
int main() {
    int N; cin >> N;
    set<int> frd;
    set<int> ffrd;
    while (N--) {
        int x; cin >> x; frd.insert(x);
        int M; cin >> M;
        while (M--) {
            int y; cin >> y; ffrd.insert(y);
        }
    }
    for (set<int>::iterator It = frd.begin(); It != frd.end(); It++) {
        ffrd.erase(*It);
    }
    cout << ffrd.size() << endl;
}
