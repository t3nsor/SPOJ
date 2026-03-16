// 2026-03-15
// The obvious recursive approach to getting from the initial state to the final
// state is to find the radius `r` of the largest disk whose position differs
// between the two states, then solve two subproblems: in the first, all disks
// smaller than `r` are moved from their positions in the initial state to the
// needle that is not occupied by disk `r` in either the initial or final
// state, and the second is the same but with the final state.  Then, the
// desired sequence of moves is the solution to the first subproblem, then move
// disk `r`, then the reverse solution to the second subproblem.
//
// However, in a recursion of the form f(p) = f(p_1) + 1 + f(p_2) (where p_1,
// p_2 are the subproblems) and the base case is f(p) = 0 or f(p) = 1, the time
// to compute f(p) will be Omega(f(p)), which is too slow.  But note that in the
// original Towers of Hanoi problem, the two subproblems are always equivalent
// up to isomorphism, so we don't have to recurse all the way down both
// subproblem subtrees.  Our assumption (proof left to exercise for the reader)
// is that memoizing subproblems will result in an efficient solution even for
// arbitrary initial and final states.  Two subproblems are isomorphic if,
// ignoring all 0 or more largest disks that don't need to be moved, the
// configurations of the disks on the remaining needles are identical up to
// permutation of the needles or exchange of the initial and final states.
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
using namespace std;

struct Subproblem {
    array<int, 6> n;

    Subproblem() = default;

    int& operator[](int i) { return n[i]; }
    const int& operator[](int i) const { return n[i]; }

    friend bool operator==(const Subproblem& p1, const Subproblem& p2)
    {
        return p1.n == p2.n;
    }
};

namespace std {
template <>
struct hash<Subproblem> {
    size_t operator()(const Subproblem& p) const
    {
        string str(24, 0);
        memcpy(&str[0], &p, sizeof(p));
        return hash<string>()(str);
    }
};
}

unordered_map<Subproblem, int> memo;

int get(const Subproblem& p) {
    return memo.insert(make_pair(p, -1)).first->second;
}

void store(Subproblem p, int val) {
    for (int i = 0; i < 2; i++) {
        int a[3] = {0, 1, 2};
        Subproblem p2;
        do {
            for (int j = 0; j < 3; j++) {
                p2[j] = p[a[j]];
                p2[j + 3] = p[3 + a[j]];
            }
            memo[p2] = val;
        } while (next_permutation(a, a + 3));
        swap(p[0], p[3]);
        swap(p[1], p[4]);
        swap(p[2], p[5]);
    }
}

int solve(Subproblem p, int ht) {
    if (ht == 0) return 0;
    int l1, l2;
    for (int i = 0; i < 3; i++) {
        if (p[i] & (1 << (ht - 1))) {
            l1 = i;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (p[i + 3] & (1 << (ht - 1))) {
            l2 = i;
        }
    }
    if (l1 == l2) {
        p[l1] -= 1 << (ht - 1);
        p[l2 + 3] -= 1 << (ht - 1);
        return solve(p, ht - 1);
    }
    int r = get(p);
    if (r < 0) {
        const int l3 = 3 - l1 - l2;
        int s[3];
        s[l3] = (1 << (ht - 1)) - 1;
        Subproblem p1 = p, p2 = p;
        p1[3] = p1[4] = p1[5] = 0;
        p1[3 + l1] = 1 << (ht - 1);
        p1[3 + l3] = (1 << (ht - 1)) - 1;
        p2[0] = p2[1] = p2[2] = 0;
        p2[l2] = 1 << (ht - 1);
        p2[l3] = (1 << (ht - 1)) - 1;
        r = solve(p1, ht - 1) + solve(p2, ht - 1) + 1;
        store(p, r);
    }
    return r;
}

int main() {
    for (;;) {
        int N; cin >> N;
        if (N == 0) break;
        Subproblem p {};
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < N; j++) {
                int x; cin >> x;
                p[3*i + x - 1] |= 1 << j;
            }
        }
        cout << solve(p, N) << '\n';
    }
}
