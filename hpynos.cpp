// 2014-04-25
#include <iostream>
#include <set>
using namespace std;
int f(int x) {
    int res = 0;
    while (x > 0) {
        res += (x%10)*(x%10);
        x /= 10;
    }
    return res;
}
int main() {
    int N;
    cin >> N;
    int res = 0;
    set<int> S;
    while (N != 1 && S.find(N) == S.end()) {
        S.insert(N);
        N = f(N);
        res++;
    }
    if (N == 1) {
        cout << res << endl;
    } else {
        cout << "-1" << endl;
    }
}
