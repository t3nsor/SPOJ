// 2016-04-28
#include <cstdio>
#include <vector>
using namespace std;
int input() {
    int res = 0; char c;
    do {
        c = getchar_unlocked();
    } while (c <= 32);
    do {
        res = res*10 + c - '0';
        c = getchar_unlocked();
    } while (c > 32);
    return res;
}
int main() {
    int n = input();
    long long total = 0;
    vector<int> stk;
    stk.push_back(input());
    for (int i = 1; i < n; i++) {
        int x = input();
        if (x >= stk.back()) {
            while (stk.size() >= 1 && x >= stk.back()) {
                stk.pop_back();
                if (stk.empty() || x < stk.back()) {
                    total += x;
                } else {
                    total += stk.back();
                }
            }
        }
        stk.push_back(x);
    }
    for (int i = 0; i+1 < stk.size(); i++) {
        total += stk[i];
    }
    printf("%lld\n", total);
}
