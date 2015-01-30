// 2014-05-01
#include <cstdio>
#include <vector>
using namespace std;
// Note: The array is 0-based.
struct BIT_range {
    int len;
    vector<long long> dataAdd;
    vector<long long> dataMul;
    // Initializes array to all zeroes
    BIT_range(int len): len(len), dataAdd(len, 0), dataMul(len, 0) {}
    void internalUpdate(int at, long long mul, long long add) {
        while (at < len) {
            dataMul[at] += mul;
            dataAdd[at] += add;
            at |= (at + 1);
        }
    }
    // Adds x to all elements in range [left, right]
    void update(int left, int right, long long x) {
        internalUpdate(left, x, -x * (left - 1));
        internalUpdate(right, -x, x * right);
    }
    // Returns sum of elements in range [0, at]
    long long query(int at) {
        long long mul = 0;
        long long add = 0;
        int start = at;
        while (at >= 0) {
            mul += dataMul[at];
            add += dataAdd[at];
            at = (at & (at + 1)) - 1;
        }
        return mul * start + add;
    }
};
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        int N, C; scanf("%d %d", &N, &C);
        BIT_range bit(N);
        while (C--) {
            int cmd; scanf("%d", &cmd);
            if (cmd == 0) {
                int p, q, v; scanf("%d %d %d", &p, &q, &v);
                bit.update(p-1, q-1, v);
            } else {
                int p, q; scanf("%d %d", &p, &q);
                printf("%lld\n", bit.query(q-1) - bit.query(p-2));
            }
        }
    }
    return 0;
}
