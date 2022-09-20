// 2022-09-20
// This (very complicated) solution explicitly computes all possible places that
// the smallest element could end up for every possible subsequence of
// instructions (i.e., the entire input sequence, and each subsequence of length
// m-1). I couldn't prove that it actually runs in linear time, but I also
// couldn't easily construct any examples where it takes a pathologically long
// time.
//
// You can find the official solution here:
//
//   https://hsin.hr/cepc2001/zadaci/rjesenja.zip (problem E)
//
// The official solution is much simpler. It relies on the fact that we don't
// need to compute all `m+1` possible ending sets, but only two sets: the set
// `polje` of possible places that the smallest element could end up if all
// instructions are executed, and the set `ora` of possible places that the
// smallest element could end up if at most one instruction was omitted (so the
// transition rule from `polje` to `ora` allows us to omit the current
// instruction, but the transition rule from `ora` to `ora` doesn't). Finally,
// the result is just |polje| + |ora| (note that this corresponds to
// |S| + |S union Y| in the notation of my solution, which is (doh) a much
// simpler way of writing 2|S| + |Y| - |S intersect Y|).
#include <iostream>
#include <unordered_set>
#include <vector>
using namespace std;
void do_testcase() {
    int n, m; cin >> n >> m;
    vector<int> as;
    vector<int> bs;
    for (int i = 0; i < m; i++) {
        int a, b; cin >> a >> b; a--; b--;
        as.push_back(a);
        bs.push_back(b);
    }
    unordered_set<int> S;
    for (int i = 0; i < n; i++) S.insert(i);
    unordered_set<int> D;
    unordered_set<int> A;
    vector<unordered_set<int>> D_for_A(n);
    vector<unordered_set<int>> A_for_D(n);
    bool can_be_unchanged = false;
    for (int i = 0; i < m; i++) {
        const int a = as[i];
        const int b = bs[i];
        if (S.count(b)) {
            if (S.count(a)) {
                if (D.count(a)) {
                    D.erase(a);
                    can_be_unchanged = true;
                }
                if (D.count(b)) {
                    D.erase(b);
                    can_be_unchanged = true;
                }
                auto it = A_for_D[a].begin();
                while (it != A_for_D[a].end()) {
                    const int p = *it;
                    it = A_for_D[a].erase(it);
                    D_for_A[p].erase(a);
                    A.insert(p);
                }
                it = A_for_D[b].begin();
                while (it != A_for_D[b].end()) {
                    const int p = *it;
                    it = A_for_D[b].erase(it);
                    D_for_A[p].erase(b);
                    A.insert(p);
                }
                A.insert(b);
                S.erase(b);
            } else {
                if (A.count(a)) {
                    A.erase(a);
                    can_be_unchanged = true;
                }
                if (D.count(b)) {
                    D.insert(a);
                    D.erase(b);
                }
                if (D_for_A[a].count(b)) {
                    D_for_A[a].erase(b);
                    A_for_D[b].erase(a);
                    can_be_unchanged = true;
                }
                auto it = D_for_A[a].begin();
                while (it != D_for_A[a].end()) {
                    const int q = *it;
                    it = D_for_A[a].erase(it);
                    A_for_D[q].erase(a);
                    D.insert(q);
                }
                it = A_for_D[b].begin();
                while (it != A_for_D[b].end()) {
                    const int p = *it;
                    it = A_for_D[b].erase(it);
                    D_for_A[p].erase(b);
                    A_for_D[a].insert(p);
                    D_for_A[p].insert(a);
                }
                A_for_D[a].insert(b);
                D_for_A[b].insert(a);
                S.erase(b);
                S.insert(a);
            }
        } else {
            if (S.count(a)) {
                A.erase(b);
                A_for_D[a].erase(b);
                D_for_A[b].erase(a);
                auto it = D_for_A[b].begin();
                while (it != D_for_A[b].end()) {
                    const int q = *it;
                    it = D_for_A[b].erase(it);
                    A_for_D[q].erase(b);
                    D.insert(q);
                }
            } else {
                if (A.count(b)) {
                    A.erase(b);
                    A.insert(a);
                }
                auto it = D_for_A[b].begin();
                while (it != D_for_A[b].end()) {
                    const int q = *it;
                    it = D_for_A[b].erase(it);
                    A_for_D[q].erase(b);
                    D_for_A[a].insert(q);
                    A_for_D[q].insert(a);
                }
            }
            can_be_unchanged = true;
        }
    }
    // S is the set of possible positions if all comparators are taken
    // Y is the set of possible positions if one comparator is omitted
    unordered_set<int> Y = A;
    unordered_set<int> Z;
    for (const auto& As : A_for_D) {
        for (const int p : As) {
            Y.insert(p);
        }
    }
    // A position that is in `S` is also in `Y` unless all modifications to
    // `S` that are possible by omitting one instruction would delete it. Note
    // that there is at most one position `p` that has this property; `p` may be
    // in `D`, and `A_for_D[q]` must be empty for all `q != p`.
    // (Note: I'm not sure whether such `p` can ever exist, but it doesn't hurt
    // to try to account for it. If `Y` is always a superset of `X`, then the
    // below formula is still correct.)
    int missing_pos = -1;
    if (!can_be_unchanged && A.empty() && D.size() <= 1) {
        int unique_d = D.empty() ? -1 : *D.begin();
        vector<int> nonempty_A_for_D;
        for (int i = 0; i < A_for_D.size(); i++) {
            if (!A_for_D[i].empty()) {
                nonempty_A_for_D.push_back(i);
            }
        }
        if (nonempty_A_for_D.empty()) {
            missing_pos = unique_d;
        } else if (nonempty_A_for_D.size() == 1 &&
                   (unique_d == -1 || nonempty_A_for_D[0] == unique_d)) {
            missing_pos = nonempty_A_for_D[0];
        }
    }
    for (const auto p : S) {
        if (p != missing_pos) {
            Y.insert(p);
            Z.insert(p);
        }
    }
    S.erase(0);
    Y.erase(0);
    Z.erase(0);
    cout << 2 * S.size() + Y.size() - Z.size() << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int d; cin >> d;
    while (d--) do_testcase();
}
