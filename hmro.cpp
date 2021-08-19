// 2021-08-19
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;
unsigned compress(unsigned node,
                  unordered_map<unsigned, unsigned>& parent) {
    unsigned p;
    while ((p = parent[node]) != node) {
        const auto pp = parent[p];
        parent[node] = pp;
        node = pp;
    }
    return node;
}
void do_testcase() {
    int p; cin >> p;
    unordered_map<long long, unsigned> boys;
    unordered_map<unsigned, unsigned> mro_parent;
    while (p--) {
        long long pesel; cin >> pesel;
        cin.get();
        unsigned mro = 0;
        for (int i = 0; i < 4; i++) {
            mro <<= 8;
            mro += cin.get();
        }
        boys[pesel] = mro;
        mro_parent[mro] = mro;
    }
    int z; cin >> z;
    while (z--) {
        unsigned from_mro = 0, to_mro = 0;
        // watch for carriage returns
        while (cin.get() != '\n');
        for (int i = 0; i < 4; i++) {
            from_mro <<= 8;
            from_mro += cin.get();
        }
        cin.get();
        for (int i = 0; i < 4; i++) {
            to_mro <<= 8;
            to_mro += cin.get();
        }
        const auto from_root = compress(from_mro, mro_parent);
        const auto to_root = compress(to_mro, mro_parent);
        mro_parent[from_root] = to_root;
    }
    cin >> p;
    char mro_str[6] = {0, 0, 0, 0, '\n', 0};
    while (p--) {
        long long pesel; cin >> pesel;
        auto mro = compress(boys[pesel], mro_parent);
        cout << pesel << ' ';
        for (int i = 3; i >= 0; i--) {
            mro_str[i] = mro & 255;
            mro >>= 8;
        }
        cout << mro_str;
    }
}
int main() {
    ios::sync_with_stdio(false);
    int s; cin >> s;
    while (s--) {
        do_testcase();
        cout << '\n';
    }
}
