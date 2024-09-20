// 2024-09-20
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;
void do_testcase(int R, int) {
    string s;
    vector<unsigned long long> a;
    for (int i = 0; i < R; i++) {
        cin >> s;
        if (i % 2) {
            s.erase(0, s.find_first_not_of("."));
            a.push_back(stoll(s));
        }
    }
    unsigned long long p = 1, q = 1;
    for (int i = a.size() - 1; i >= 0; i--) {
        swap(p, q);
        p += a[i] * q;
    }
    cout << p << ' ' << q << '\n';
}
int main() {
    int R, C;
    for (;;) {
        cin >> R >> C;
        if (R == 0) return 0;
        do_testcase(R, C);
    }
}
