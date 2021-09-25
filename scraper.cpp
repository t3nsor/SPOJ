// 2021-09-24
#include <cstdlib>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;
// requires a, b >= 0
// returns gcd(a, b) and sets x, y such that ax + by = gcd
int extended_euclid(int a, int b, int& x, int& y) {
    if (a == 0) {
        x = 0; y = 1; return b;
    }
    int x2, y2;
    const int result = extended_euclid(b%a, a, x2, y2);
    // (b%a)*x2 + a*y2 = result
    // -> (b%a)*x2 + (a*(b/a))*x2 + a*y2 = result + (a*(b/a))*x2
    // -> b*x2 + a*y2 = result + (a*(b/a))*x2
    // -> a*(y2-(b/a)*x2) + b*x2 = result
    y = x2;
    x = y2 - (b/a) * x2;
    return result;
}
// finds least nonnegative x such that ax \equiv b (mod m)
// requires m > 0
bool solve(int a, int b, int m, int& x) {
    // first, solve ay + mz = gcd(a, m)
    int y, z;
    const int d = extended_euclid(abs(a), m, y, z);
    if (a < 0) y *= -1;
    // It follows that ay \equiv gcd(a, m) (mod m)
    if (b % d) return false;
    long long xx = y;
    xx *= b / d;
    // reduce it modulo m/gcd
    x = ((xx % (m/d)) + m/d) % (m/d);
    return true;
}
bool can_transfer(int X1, int Y1, int X2, int Y2, int F) {
    // WLOG, Y1 >= Y2
    if (Y1 < Y2) return can_transfer(X2, Y2, X1, Y1, F);
    // need Y1 + x*X1 \equiv Y2 (mod X2), with x >= 0.
    // -> X1 * x \equiv Y2 - Y1 (mod X2)
    int x;
    if (!solve(X1, Y2 - Y1, X2, x)) return false;
    return Y1 + x * X1 < F;
}
bool do_testcase() {
    int F, E, A, B; cin >> F >> E >> A >> B;
    vector<int> X(E), Y(E);
    vector<int> target(E);
    stack<int> stk;
    for (int i = 0; i < E; i++) {
        cin >> X[i] >> Y[i];
        if (Y[i] <= B && (B - Y[i]) % X[i] == 0) {
            // elevator stops at floor B; mark it as a target
            target[i] = 1;
        }
        if (Y[i] <= A && (A - Y[i]) % X[i] == 0) {
            // elevator stops at floor A; mark it as a source for the DFS
            stk.push(i);
        }
    }
    if (A == B) return true;  // special case where no elevator required
    vector<int> vis(E, 0);
    while (!stk.empty()) {
        int i = stk.top(); stk.pop();
        if (vis[i]) continue; vis[i] = 1;
        if (target[i]) return true;
        for (int j = 0; j < E; j++) {
            if (j == i || vis[j]) continue;
            if (can_transfer(X[i], Y[i], X[j], Y[j], F)) {
                stk.push(j);
            }
        }
    }
    return false;
}
int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;
    while (N--) {
        if (do_testcase()) {
            cout << "It is possible to move the furniture.\n";
        } else {
            cout << "The furniture cannot be moved.\n";
        }
    }
}
