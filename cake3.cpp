// 2026-05-08
// The genre of problem is immediately recognizable.  This is a DP where the
// state encodes information about the rightmost column of the cake so far.  We
// build up the cake by adding a new rightmost column repeatedly.  However, this
// is probably the hardest problem of this genre that I've ever seen.  Here are
// two ideas that DON'T work.
//
// First, you might think that the only cuts that we need to avoid counting are
// the ones that have a "loose end": an interior lattice point for which exactly
// one of the four possible cut segments is actually cut.  However, that is
// incorrect because we also must not count cuts such as:
//
// .--.--.--.
// |     |  |
// .  .--.  .
// |  |  |  |
// .  .--.  .
// |        |
// .--.--.--.
//
// The above configuration is inadmissible because the slice consisting of the
// outer eight squares has a cut segment that passes between two of its squares.
//
// Evidently, the DP state has to encode information about which connected
// component each square in the rightmost column belongs to so that we can
// ensure we don't count any cuts that contain an illegal segment.  Therefore,
// it's tempting to say that the DP state represents an assignment of each
// square in the rightmost column to a connected component based on the cake so
// far (i.e., taking into account the part of the cake to the left, but not the
// part to the right because we haven't gotten there yet) and we have to
// exclude possible ways to add the next column that would place an illegal
// segment (possibly as a result of merging two components).  But this also
// isn't quite right, because if two squares belong to different components
// according to what we have so far, but then are merged by the addition of the
// next column, the merge may be illegal because there was a cut segment
// between the two to-be-merged components somewhere to the left.
//
// So here's what we do: the DP state consists of a sequence of N ordered pairs
// (c, r), representing the rightmost column.  The ith entry gives information
// about what component the ith square in the column belongs to:
// * Two squares in the same column are assigned the same c-value if and only if
//   in the final cake (including columns not yet added to the right) they are
//   in the same component.
// * Two squares with the same c-value are assigned the same r-value if and only
//   if, in the part of the cake seen so far (including this column), the fact
//   that they're part of the same component has already been "ratified" (i.e.
//   because there is a path between them in this part of the cake that doesn't
//   pass through any cuts).
// We must reject DP transitions that leave connectedness relationships
// unratified.  For example, if the first column is
//
// (0, 0)
// (1, 0)
// (0, 1)
//
// meaning that the first and last squares are in the same component, and the
// middle square is in a different component, then we cannot transition to
//
// (1, 0)
// (1, 0)
// (1, 0)
//
// because that would mean the first and last squares in the first column never
// ended up joined with each other even though we claimed that they would be in
// the same component.  Similarly, when we complete the outermost loop of the
// DP, we discard final states that have any nonzero r-values.
//
// Note that the official solution is a bit different: the DP state there is a
// partition of the rightmost column into components based on the cake so far,
// plus a boolean function of unordered pairs of such components indicating
// whether we have seen a cut segment between them.  (So if two such components
// would eventually be joined into the same component, we must reject such a
// transition at that point if there is already a cut between them.)  This
// sounds more complicated, but ends up producing a much shorter solution
// (though it seems that it doesn't handle bignums):
// https://ipsc.ksp.sk/2007/real/solutions/d.cpp

#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <stack>
#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

struct Bignum {
    static constexpr unsigned long long RADIX = 1e18;
    vector<unsigned long long> digits;
    Bignum(unsigned int x = 0) : digits{x} {}
    void add(const Bignum& other) {
        digits.resize(max(digits.size(), other.digits.size()) + 1);
        int carry = 0;
        for (int i = 0; i < digits.size(); i++) {
            digits[i] = digits[i] + carry;
            if (i < other.digits.size()) digits[i] += other.digits[i];
            if (digits[i] >= RADIX) {
                carry = 1;
                digits[i] -= RADIX;
            } else {
                carry = 0;
            }
        }
        while (digits.size() > 1 && digits.back() == 0) digits.pop_back();
    }
    void print() const {
        printf("%lld", digits.back());
        for (int i = (int)digits.size() - 2; i >= 0; i--) {
            printf("%018lld", digits[i]);
        }
    }
};

// Represents a possible state of the rightmost column.  `State` objects are
// always in canonical form, which means they represent a sequence of (c, r)
// pairs, where c-values are assigned sequentially starting from 0, the first
// occurrence of a c-value is earlier than the first occurrence of any larger
// c-value, and for a given c-value, the properties above are satisfied by the
// r-values of the entries with that c-value.
struct State {
    unsigned int val;
    pair<int, int> operator[](int i) const {
        const int entry = (val >> (6*i)) & ((1 << 6) - 1);
        return {entry >> 3, entry & ((1 << 3) - 1)};
    }

    // A state for which all c-values and r-values are zero (representing a
    // column with no horizontal cuts).
    State() : val(0) {}

    // Construct a `State` from a sequence of pairs which need not be in
    // canonical form.
    template <class It>
    State(It first, It last) : val(0) {
        map<int, pair<int, map<int, int>>> id;
        for (It i = first; i != last; i++) {
            if (!id.count(i->first)) id[i->first].first = id.size();
            if (!id[i->first].second.count(i->second)) {
                id[i->first].second[i->second] = id[i->first].second.size();
            }
        }
        int scale = 0;
        for (It i = first; i != last; i++) {
            const int c = id[i->first].first;
            const int r = id[i->first].second[i->second];
            val |= ((c << 3) + r) << scale;
            scale += 6;
        }
    }

    friend bool operator==(State s1, State s2) {
        return s1.val == s2.val;
    }
};

namespace std {
template <>
struct hash<State> {
    size_t operator()(State s) const {
        return hash<unsigned int>{}(s.val);
    }
};
}

// Return a list of new canonical states obtained from the given state `s` if
// vertical cuts are placed to the right of the column represented by `s`
// according to `vmask`, then horizontal cuts are placed in a new column to the
// right of `s` according to `hmask`.  Note that the returned list can have more
// than one element (in case there is a possibility that multiple new components
// start in the new column); it can also be empty if the combination of
// arguments is invalid.  An invalid combination can occur for the following
// reasons:
// * Two different components (according to c-values) are merged.
// * A cut is placed between two squares that are in the same component.
// * There is a c-value in `s` that has more than one r-value, and at least one
//   such r-value is not adjacent to any square in the result (making
//   ratification impossible).
vector<State> calc_transitions(int n, State s, int vmask, int hmask) {
    // New column being built up
    vector<pair<int, int>> col(n, {-1, -1});
    // Tracks merges between r-values for a given c-value.  An edge points from
    // the larger r-value to the smaller r-value, so at the end we just follow
    // the edges to reach the smallest r-value possible, which identifies the
    // resulting ratified component.
    int edge[5][5]; memset(edge, -1, sizeof(edge));
    // Tracks which r-values (for a given c-value) are found in the new column.
    map<int, bool> haver[5];
    for (int i = 0; i < n; i++) {
        auto start = s[i];
        auto& h = haver[start.first][start.second];
        if (vmask & (1 << i)) continue;
        haver[start.first][start.second] = true;
        stack<int> stk;
        stk.push(i);
        while (!stk.empty()) {
            int row = stk.top();
            stk.pop();
            bool must_expand = false;
            if (col[row].first == -1) {
                must_expand = true;
                col[row].first = start.first;
            } else if (col[row].first != start.first) {
                // components would merge
                return {};
            }
            if (col[row].second == -1) {
                col[row].second = start.second;
            } else {
                int small = min(col[row].second, start.second);
                int big = max(col[row].second, start.second);
                if (small != big) edge[start.first][big] = small;
            }
            if (!must_expand) continue;
            if (row > 0 && (hmask & (1 << (row - 1))) == 0) {
                stk.push(row - 1);
            }
            if (row + 1 < n && (hmask & (1 << row)) == 0) {
                stk.push(row + 1);
            }
        }
    }
    // check for r-values that didn't make it across
    for (int c = 0; c < 5; c++) {
        if (haver[c].size() <= 1) continue;
        for (const auto& kv : haver[c]) {
            if (!kv.second) return {};
        }
    }
    // assign r-values for visited squares
    for (int i = 0; i < n; i++) {
        if (col[i].first >= 0) {
            while (edge[col[i].first][col[i].second] != -1) {
                col[i].second = edge[col[i].first][col[i].second];
            }
        }
    }

    // Check whether any cuts are between two squares in the same component.
    auto is_valid = [&](const auto& col2) {
        for (int i = 0; i < n - 1; i++) {
            if ((hmask & (1 << i)) > 0 && col2[i].first == col2[i + 1].first) {
                return false;
            }
        }
        for (int i = 0; i < n; i++) {
            if ((vmask & (1 << i)) > 0 && s[i].first == col2[i].first) {
                return false;
            }
        }
        return true;
    };

    // Assign values for unvisited squares.  We assign these starting from 5 to
    // avoid conflict with the c-values from `s`.

    // number of unvisited segments in new column
    int m = 0;
    // vector of (c, n(c)) pairs where c is a c-value from `s` that made it into
    // `col`, and n(c) is the number of ratified components of c present in
    // `col` (note that the r-values were already made sequential in the
    // previous step)
    vector<pair<int, int>> oldcnt;
    if (col[0].first >= 0) {
        oldcnt.emplace_back(col[0].first, col[0].second + 1);
    } else {
        m = 1;
    }
    for (int i = 0; i < n - 1; i++) {
        if ((hmask & (1 << i)) > 0) {
            if (col[i + 1].first >= 0) {
                bool found = false;
                for (int j = 0; j < oldcnt.size(); j++) {
                    if (oldcnt[j].first == col[i + 1].first) {
                        oldcnt[j].second = max(oldcnt[j].second,
                                               col[i + 1].second + 1);
                        found = true;
                    }
                }
                if (!found) {
                    oldcnt.emplace_back(col[i + 1].first,
                                        col[i + 1].second + 1);
                }
            } else {
                m++;
            }
        }
    }
    vector<State> result;
    int pwr = 1;
    for (int i = 0; i < m; i++) {
        pwr *= m + oldcnt.size() + 1;
    }
    for (int i = 0; i < pwr; i++) {
        int i2 = i;
        auto col2 = col;
        auto oldcnt2 = oldcnt;
        vector<int> newcnt;
        bool valid = true;
        for (int j = 0; j < n; j++) {
            if (j > 0 && (hmask & (1 << (j - 1))) == 0) {
                col2[j] = col2[j - 1];
                continue;
            }
            if (col2[j].first >= 0) continue;
            int choice = i2 % (m + oldcnt.size() + 1);
            i2 /= (m + oldcnt.size() + 1);
            if (choice < oldcnt.size()) {
                // reuse a c-value from `s`, but assign the next r-value
                col2[j] = oldcnt2[choice];
                oldcnt2[choice].second++;
                continue;
            }
            choice -= oldcnt.size();
            if (choice < newcnt.size()) {
                // reuse one of the new c-values
                col2[j] = make_pair(choice + 5, newcnt[choice]++);
            } else if (choice == newcnt.size()) {
                // start a new c-value
                newcnt.push_back(1);
                col2[j] = make_pair(choice + 5, 0);
            } else {
                valid = false;
                break;
            }
        }
        if (valid && is_valid(col2)) {
            result.push_back(State(col2.begin(), col2.end()));
        }
    }
    return result;
}

const vector<State>& transitions(int n, State s, int vmask, int hmask) {
    static unordered_map<unsigned long long, vector<State>> memo;
    unsigned long long key = ((unsigned long long)s.val << 15) +
                             (n << 10) + (vmask << 5) + hmask;
    auto r = memo.insert({key, {}});
    if (r.second) r.first->second = calc_transitions(n, s, vmask, hmask);
    return r.first->second;
}

void do_testcase(int N, int M) {
    unordered_map<State, Bignum> dp;
    // To prepare the dp values for the first column, we pretend there's a
    // column to the left of the first column and try all possibilities for the
    // horizontal cuts in the actual first column.

    for (int hmask = 0; hmask < (1 << (N - 1)); hmask++) {
        for (State s : transitions(N, State(), (1 << N) - 1, hmask)) {
            dp.emplace(s, 1);
        }
    }

    for (int i = 0; i < M - 1; i++) {
        unordered_map<State, Bignum> dp2;
        for (const auto& kv : dp) {
            for (int vmask = 0; vmask < (1 << N); vmask++) {
                for (int hmask = 0; hmask < (1 << (N - 1)); hmask++) {
                    const auto& new_states =
                      transitions(N, kv.first, vmask, hmask);
                    for (const auto ns : new_states) dp2[ns].add(kv.second);
                }
            }
        }
        dp = move(dp2);
    }

    Bignum result;
    for (const auto& kv : dp) {
        bool valid = true;
        for (int i = 0; i < N; i++) {
            if (kv.first[i].second != 0) {
                // unratified component
                valid = false;
                break;
            }
        }
        if (valid) result.add(kv.second);
    }
    result.print();
    putchar('\n');
}

int main() {
    int T; scanf("%d", &T);
    while (T--) {
        int N, M; scanf("%d %d", &N, &M);
        if (N > M) swap(N, M);
        do_testcase(N, M);
    }
}
