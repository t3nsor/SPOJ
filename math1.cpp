// 2024-02-17
// The approach is greedy. First of all notice that any valid set of x's remains
// valid when all x's are shifted by the same offset. And for any valid set of
// x's,
//   \sum_i |x_i - m| = \min_k \sum_i |x_i - k|
// That is to say, the sum of differences between the x's and their median
// equals the most favourable possible sum of absolute values (i.e. the minimum
// possible sum of absolute values achievable by shifting the x's).
//
// Now, given the array a_1, ..., a_n we initially set
//   x_0 = 0
//   x_1 = a_1 - 1
//   x_2 = a_1 + a_2 - 2
//   ...
//   x_n = a_1 + ... + a_n - n
// (though in the solution below we simply overwrite `a`). This isn't actually a
// valid array (except when a_1, ..., a_n are all zero) as it doesn't satisfy
// the constraint on the difference between x_1 and x_n. We need to make it
// valid by incrementing exactly (n - a_1 - ... - a_n) of the differences (i.e.,
// from a_i - 1 to a_i) so that we finally end up with x_n = 0 and either
// x_1 = a_1 - 1 or x_1 = a_1.
//
// That is, we repeatedly pick some i (1 <= i <= n) and increment the difference
// between x_{i-1} and x_i by incrementing x_i, ..., x_n. At each step, we pick
// the i such that the resulting value of \sum_i |x_i - m| will be as small as
// possible (greedy). We're not allowed to pick the same i more than once during
// the algorithm, but it turns out that doing so never gives a better solution
// anyway (so we don't need to enforce it; see the commented-out lines below).
//
// Now the main question is how to do this simulation efficiently. Since all x_i
// values are always between -n and n-1 (inclusive) we maintain an array that
// tracks how many times each value betwen -n and n-1 occurs in the x array. We
// also need to keep track of the current median and some additional metadata
// that enables the calculation of \sum_i |x_i - m|. Updating this data
// structure can be done in O(1) when one of the x_i's is incremented. Finally,
// notice that we only need to perform n incrementations in order to explore all
// the possibilities for a single iteration (since picking i = n increments only
// x_n, picking i = n - 1 increments both x_{n-1} and x_n, and so on). This
// gives an overall O(n^2) algorithm.

#include <iostream>
#include <string.h>
#include <vector>
using namespace std;

struct Median {
    int freq_data_[2001];
    int* freq_ = freq_data_ + 1001;
    int n_;
    int lcnt_, rcnt_, lsum_, rsum_;
    int m_;  // either the median or (in case of even n_) the largest element
             // less than the median

    Median() = default;

    template <class It>
    Median(It begin, It end) : n_(end - begin) {
        memset(freq_data_, 0, sizeof(freq_data_));
        for (It i = begin; i != end; i++) {
            freq_[*i]++;
        }
        lcnt_ = 0;
        rcnt_ = n_ - freq_[-n_];
        m_ = -n_;
        while (rcnt_ > n_ / 2) {
            lcnt_ += freq_[m_];
            m_++;
            rcnt_ -= freq_[m_];
        }
        lsum_ = 0;
        for (int i = -n_; i < m_; i++) {
            lsum_ += i * freq_[i];
        }
        rsum_ = 0;
        for (int i = m_ + 1; i < n_; i++) {
            rsum_ += i * freq_[i];
        }
    }

    // Return sum of |x_i - m| where m is the median.
    int calc() const {
        const int lsum2 = lsum_ + (n_ / 2 - lcnt_) * m_;
        const int rsum2 = rsum_ + (n_ / 2 - rcnt_) * m_;
        return rsum2 - lsum2;
    }

    void inc(int x) {
        freq_[x]--;
        freq_[x + 1]++;
        if (x < m_ - 1) {
            lsum_++;
        } else if (x == m_ - 1) {
            lcnt_--;
            lsum_ -= x;
        } else if (x == m_) {
            rcnt_++;
            rsum_ += x + 1;
        } else {
            rsum_++;
        }
        while (rcnt_ > n_ / 2) {
            lcnt_ += freq_[m_];
            lsum_ += m_ * freq_[m_];
            m_++;
            rcnt_ -= freq_[m_];
            rsum_ -= m_ * freq_[m_];
        }
    }
};

void do_testcase() {
    int n; cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        a[i]--;
        if (i > 0) a[i] += a[i - 1];
    }
    int iterations = -a[n - 1];
    // vector<int> used(n, 0);
    Median median(a.begin(), a.end());
    while (iterations--) {
        int best_cost = 1e9;
        int best_index;
        Median best_array;
        for (int i = n - 1; i >= 0; i--) {
            median.inc(a[i]);
            // if (used[i]) continue;
            const int val = median.calc();
            if (val < best_cost) {
                best_cost = val;
                best_index = i;
                best_array = median;
            }
        }
        // used[best_index] = 1;
        for (int i = best_index; i < n; i++) {
            a[i]++;
        }
        median = best_array;
    }
    cout << median.calc() << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
