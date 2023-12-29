// 2023-12-28
// We use the "segment tree where each node contains a BIT" approach. A BIT for
// a particular subarray of the original array contains a number of elements
// equal to the number of distinct values that are held by any elements of that
// subarray over the life of the test case (i.e. both initial and non-initial
// values). This uses O(N log K) space where K is the total number of different
// values in the array over the life of the test case (K <= 50000). We use a
// technique that initializes each BIT in linear (rather than loglinear) time,
// so the total initialization time can be O(N log K), not O(N log^2 K). When
// initializing the segment tree, we use merge sort both to determine the set of
// values that each BIT is responsible for, and to count the initial number of
// inversions.
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
const int INF = 1e9;
struct RLEList {
    vector<int> values;
    vector<int> counts;
    int size() const { return values.size(); }
};
struct Node {
    vector<int> vals;
    vector<int> BIT;
    void init(const RLEList& l) {
        vals = l.values;
        BIT = l.counts;
        for (int i = 0; i < BIT.size(); i++) {
            if ((i | (i + 1)) < BIT.size()) {
                BIT[i | (i + 1)] += BIT[i];
            }
        }
    }
    int query_BIT(int idx) const {
	int res = 0;
	while (idx >= 0) {
	    res += BIT[idx];
	    idx = (idx & (idx + 1)) - 1;
	}
	return res;
    }
    void upd_BIT(int idx, int diff) {
	while (idx < vals.size()) {
	    BIT[idx] += diff;
	    idx = idx | (idx + 1);
	}
    }
    // Count elements strictly less than `x`.
    int query(int x) const {
	return query_BIT(
          lower_bound(vals.begin(), vals.end(), x) - vals.begin() - 1);
    }
    // In this case `val` must be in `vals`.
    void upd(int val, int diff) {
	upd_BIT(lower_bound(vals.begin(), vals.end(), val) - vals.begin(),
                diff);
    }
};
struct Segtree {
    int size_;
    vector<Node> nodes_;
    vector<int> a_;
    long long total_inversions_ = 0;

    Segtree(const vector<RLEList>& a)
    : size_(a.size()),
      nodes_(4 * size_),
      a_(a.size()) {
        initialize(1, 0, size_, a);
    }

    RLEList merge(const RLEList& l1, const RLEList& l2) {
        RLEList result;
        int e2 = 0, i1 = 0, i2 = 0;
        while (i1 < l1.size() || i2 < l2.size()) {
            const int x1 = (i1 < l1.size() ? l1.values[i1] : INF);
            const int x2 = (i2 < l2.size() ? l2.values[i2] : INF);
            if (x1 == x2) {
                result.values.push_back(x1);
                result.counts.push_back(l1.counts[i1] + l2.counts[i2]);
                total_inversions_ += l1.counts[i1] * (long long)e2;
                e2 += l2.counts[i2];
                ++i1;
                ++i2;
            } else if (x1 < x2) {
                result.values.push_back(x1);
                result.counts.push_back(l1.counts[i1]);
                total_inversions_ += l1.counts[i1] * (long long)e2;
                ++i1;
            } else {
                result.values.push_back(x2);
                result.counts.push_back(l2.counts[i2]);
                e2 += l2.counts[i2];
                ++i2;
            }
        }
        return result;
    }

    RLEList initialize(int node, int tl, int tr, const vector<RLEList>& a) {
        RLEList l;
        if (tl + 1 == tr) {
            l = a[tl];
            for (int i = 0; i < l.size(); i++) {
                if (l.counts[i]) a_[tl] = l.values[i];
            }
        } else {
            int tm = (tl + tr) / 2;
            const RLEList l1 = initialize(2*node, tl, tm, a);
            const RLEList l2 = initialize(2*node + 1, tm, tr, a);
            l = merge(l1, l2);
        }
        nodes_[node].init(l);
        return l;
    }

    int query(int ql, int qr, int x_old, int x_new) const {
        return query(1, 0, size_, ql, qr, x_old, x_new);
    }

    int query(int node, int tl, int tr, int ql, int qr,
              int x_old, int x_new) const {
        if (tl >= ql && tr <= qr) {
            return nodes_[node].query(x_new) -
                   nodes_[node].query(x_old);
        }
        int result = 0;
        const int tm = (tl + tr) / 2;
        if (ql < tm && qr > tl) {
            result += query(2*node, tl, tm, ql, qr, x_old, x_new);
        }
        if (ql < tr && qr > tm) {
            result += query(2*node + 1, tm, tr, ql, qr, x_old, x_new);
        }
        return result;
    }

    void update(int i, int x_new) {
        const int x_old = a_[i];
        total_inversions_ += query(i + 1, size_, x_old, x_new) +
                             query(0, i, x_new + 1, x_old + 1);
        update(1, 0, size_, i, x_new);
        a_[i] = x_new;
    }

    void update(int node, int tl, int tr, int i, int x) {
        nodes_[node].upd(a_[i], -1);
        nodes_[node].upd(x, 1);
        if (tr - tl > 1) {
            const int tm = (tl + tr) / 2;
            if (i < tm) {
                update(2*node, tl, tm, i, x);
            } else {
                update(2*node + 1, tm, tr, i, x);
            }
        }
    }
};
int main() {
    int N; scanf("%d", &N);
    vector<int> initial(N);
    vector<RLEList> l(N);
    for (int i = 0; i < N; i++) {
        scanf("%d", &initial[i]);
        l[i].values.push_back(initial[i]);
    }
    int M; scanf("%d", &M);
    vector<pair<int, int>> mod(M);
    for (int i = 0; i < M; i++) {
        scanf("%d %d", &mod[i].first, &mod[i].second);
        --mod[i].first;
        l[mod[i].first].values.push_back(mod[i].second);
    }
    for (int i = 0; i < N; i++) {
        sort(l[i].values.begin(), l[i].values.end());
        l[i].values.erase(unique(l[i].values.begin(), l[i].values.end()),
                          l[i].values.end());
        l[i].counts.resize(l[i].values.size());
        for (int j = 0; j < l[i].size(); j++) {
            if (l[i].values[j] == initial[i]) {
                l[i].counts[j] = 1;
            }
        }
    }
    Segtree ST(l);
    for (const auto& p : mod) {
        ST.update(p.first, p.second);
        printf("%lld\n", ST.total_inversions_);
    }
}
