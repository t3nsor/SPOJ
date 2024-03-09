// 2024-03-09
// We use a meet-in-the-middle approach. The unknown die can have up to 6 sides,
// so we consider each possible triple of side values (in the range 1 to 50) and
// consider how many ways that triple would provide for each of the constrained
// totals, adding it to a lookup table if each number of ways does not exceed
// the desired count for that constrained total. Then, perform recursion to
// check every possible lower half of the face values. For each such
// combination, we can look up whether a precomputed triple supplies the missing
// counts.
//
// The official solution can be found here:
//   https://icpc-ecna.ysu.edu/PastResults/2004/Problem-Set/Solutions/E.cpp
// I haven't analyzed how it works, but it's interesting to note that it's a lot
// faster than my solution, but gets WA on SPOJ.
#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <limits.h>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

// We only use this convolution function at the beginning of each test case to
// replace all given input dice with one single die. At other points in the
// algorithm, it's too slow, considering that one vector will be sparse.
vector<unsigned> conv(const vector<unsigned>& v1, const vector<unsigned>& v2) {
    vector<unsigned long long> result(v1.size() + v2.size() - 1);
    for (int i = 0; i < v1.size(); i++) {
        for (int j = 0; j < v2.size(); j++) {
            result[i + j] = result[i + j] + v1[i] * v2[j];
            if (result[i + j] > INT_MAX) result[i + j] = INT_MAX;
        }
    }
    return {result.begin(), result.end()};
}

struct Key {
    array<int, 10> data;
};
bool operator==(const Key& k1, const Key& k2) { return k1.data == k2.data; }

namespace std {
template <>
struct hash<Key> {
    size_t operator()(const Key& k) const {
        size_t result = 0;
        for (unsigned i = 0; i < 10; i++) {
            result += i * k.data[i];
        }
        return result;
    }
};
}

bool rec(vector<int>& result, int f, int last,
         const vector<unsigned>& otherdie,
         const vector<pair<int, int>>& constraints,
         const unordered_map<Key, tuple<int, int, int>> lookup[]) {
    if (f == 0) {
        // Base case. Check if we have a valid combination.
        for (const auto& kv : constraints) {
            const int val = kv.first;
            const int cnt = kv.second;
            long long pval = 0;
            for (int i = 0; i < result.size(); i++) {
                if (val >= result[i] &&
                    val - result[i] < otherdie.size()) {
                    pval += otherdie[val - result[i]];
                }
            }
            if (pval != cnt) return false;
        }
        return true;
    }
    if (f == 3) {
        // Other base case. Look up a completion in the table.
        auto& M = lookup[last];
        Key remain{};
        for (int i = 0; i < constraints.size(); i++) {
            const int val = constraints[i].first;
            const int cnt = constraints[i].second;
            long long pval = 0;
            for (const int x : result) {
                if (val >= x && val - x < otherdie.size()) {
                    pval += otherdie[val - x];
                }
            }
            if (pval > cnt) return false;
            remain.data[i] = cnt - pval;
        }
        const auto it = M.find(remain);
        if (it == M.end()) return false;
        result.push_back(get<0>(it->second));
        result.push_back(get<1>(it->second));
        result.push_back(get<2>(it->second));
        return true;
    }
    // Recursive case.
    for (int i = last; i <= 50; i++) {
        result.push_back(i);
        if (rec(result, f - 1, i, otherdie, constraints, lookup)) {
            return true;
        }
        result.pop_back();
    }
    return false;
}

void do_testcase(int n) {
    // Replace all given input dice with a single die that represents the
    // convolution of them.
    vector<unsigned> otherdie = {1};
    for (int i = 0; i < n; i++) {
        int f; cin >> f;
        vector<int> die(f);
        int size = 0;
        for (int j = 0; j < f; j++) {
            cin >> die[j];
            size = max(size, die[j] + 1);
        }
        vector<unsigned> v(size);
        for (const int value : die) {
            v[value]++;
        }
        otherdie = conv(otherdie, v);
    }
    int f, c; cin >> f >> c;
    vector<pair<int, int>> constraints(c);
    for (int i = 0; i < c; i++) {
        cin >> constraints[i].first >> constraints[i].second;
    }
    sort(constraints.begin(), constraints.end());
    // check for contradictory constraints
    for (int i = 1; i < c; i++) {
        if (constraints[i].first == constraints[i - 1].first &&
            constraints[i].second != constraints[i - 1].second) {
            cout << "Impossible\n";
            return;
        }
    }
    unordered_map<Key, tuple<int, int, int>> lookup[51];
    if (f >= 3) {
        // precompute 3-face combinations
        for (int i = 50; i >= 1; i--) {
            for (int j = 50; j >= i; j--) {
                for (int k = 50; k >= j; k--) {
                    // This is one of the hot loops, so we have to optimize it.
                    // We do this by not computing the full convolution, but
                    // only the constrained points, each of which has a value
                    // that's made up of at most 3 terms.
                    Key key{};
                    for (int m = 0; m < c; m++) {
                        int val = constraints[m].first;
                        int cnt = constraints[m].second;
                        long long pval = 0;
                        if (val >= i && val - i < otherdie.size()) {
                            pval += otherdie[val - i];
                        }
                        if (val >= j && val - j < otherdie.size()) {
                            pval += otherdie[val - j];
                        }
                        if (val >= k && val - k < otherdie.size()) {
                            pval += otherdie[val - k];
                        }
                        if (pval > cnt) {
                            goto continue2;
                        }
                        key.data[m] = pval;
                    }
                    for (int m = 1; m <= i; m++) {
                        lookup[m][key] = make_tuple(i, j, k);
                    }
continue2:
                    ;
                }
            }
        }
    }

    vector<int> result;
    if (rec(result, f, 1, otherdie, constraints, lookup)) {
        cout << "Final die face values are";
        for (const int v : result) {
            cout << ' ' << v;
        }
        cout << '\n';
    } else {
        cout << "Impossible\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    int n;
    for (;;) {
        cin >> n;
        if (n == 0) return 0;
        do_testcase(n);
    }
}
