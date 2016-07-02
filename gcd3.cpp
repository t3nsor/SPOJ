// 2016-07-01
#include <iostream>
#include <deque>
using namespace std;
typedef deque<int> T;
bool e(const T& n) {
    return n.back() % 2 == 0;
}
void s(T& n) {
    int c = 0;
    for (int i = 0; i < (int)n.size(); i++) {
        int d = 10*c + n[i];
        c = d%2;
        n[i] = d/2;
    }
    if (n.front() == 0) {
        n.pop_front();
    }
}
T f(const string& s) {
    T r(s.size());
    for (int i = 0; i < (int)s.length(); i++) {
        r[i] = s[i] - '0';
    }
    return r;
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        string P, Q;
        int K;
        cin >> P >> Q >> K;
        T N = f(P), M = f(Q);
        if (e(N)) {
            int p = 0;
            while (p < K && e(N)) {
                p++;
                s(N);
            }
            cout << (1 << p) << "\n";
        } else {
            cout << "1\n";
        }
    } 
    return 0;
}
