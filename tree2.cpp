// 2022-08-22
/*
Approach:

The set of valid bitstrings is easy to characterize:
* 0, or
* 1 followed by a valid bitstring, followed by a valid bitstring.

One naive approach is to generate all bitstrings and stick them in a trie.
Going to the next bitstring would correspond to finding the next node in a
left-to-right traversal of the trie. There are 2674440 valid bitstrings of
length 29 and the number decreases rapidly with decreasing length. It's
plausible that this approach would be fast enough on the modern SPOJ grader, butwe should try to find a solution that would have worked during the actual
CEPC 2003.

Another approach is as follows: if we know how to determine whether a bitstring
is valid, then we can just keep incrementing the given bitstring until we get a
valid one. (If we start at the highest possible value, H(l) = 2^l - 2^((l+1)/2),
then we wrap around to the minimum, L(l) = (2^(l+1) - 4) / 3. To speed this up,
we note that a valid bitstring must contain exactly (l-1)/2 bits set (including
the most significant bit, which must always be set). When we restrict ourselves
to the set S = {elements of [L(l), H(l)] with exactly (l-1)/2 bits set}, then
valid strings are probably dense enough in S that we can just keep incrementing
until we find one. (Note that `l` is the length of the given bitstring.)

To quickly determine whether a bit string is valid, we can precompute the
valid bit strings of length up to 23 and use recursion for lengths > 23.

--

The official solution is more clever and is efficient for arbitrary length, but
is sort of hard to understand. You can find it here:

https://hsin.hr/cepc2003/zadaci/rjesenja.zip (file `g.c`)
*/
#include <algorithm>
#include <bitset>
#include <iostream>
#include <vector>
using namespace std;
bitset<(1<<22)> lookup_table;
void build_lookup_table() {
    vector<vector<int>> v(24);
    v[1] = vector<int>{0};
    for (int i = 3; i <= 23; i++) {
        for (int j = 1; j < i; j += 2) {
            const int k = i - j - 1;
            for (const int t1 : v[j]) {
                for (const int t2 : v[k]) {
                    const int t = (1 << (i - 1)) | (t1 << k) | t2;
                    v[i].push_back(t);
                    lookup_table[t &~ (1 << 22)] = 1;
                }
            }
        }
    }
}
bool is_valid(int l, int x) {
    if (l == 1) return x == 0;
    if (l <= 23) {
        return (x & (1 << (l - 1))) && lookup_table[x &~ (1 << 22)];
    }
    for (int i = 1; i < l; i+= 2) {
        const int j = l - i - 1;
        const int t2 = x & ((1 << j) - 1);
        const int t1 = (x &~ (1 << (l - 1))) >> j;
        if (is_valid(i, t1) && is_valid(j, t2)) return true;
    }
    return false;
}
int main() {
    ios::sync_with_stdio(false);
    build_lookup_table();
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        if (n == 0) {
            cout << "0\n";
            continue;
        }
        const int l = 32 - __builtin_clz(n);
        const int L = ((1 << (l + 1)) - 4) / 3;
        const int H = (1 << l) - (1 << ((l + 1) / 2));
        if (n == H) {
            cout << L << '\n';
            continue;
        }
        vector<int> v(l);
        for (int i = l - 1; i >= 0; i--) {
            v[i] = n & 1;
            n >>= 1;
        }
        for (;;) {
            next_permutation(v.begin(), v.end());
            n = 0;
            for (int i = 0; i < l; i++) {
                n = (n << 1) | v[i];
            }
            if ((n & 3) == 0 && is_valid(l, n)) {
                break;
            }
        }
        cout << n << '\n';
    }
}
