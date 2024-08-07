// 2024-08-06
//
// The overall approach is as follows. Iterate over all possible values of `l`
// (length of repeated substring). For each such l, consider pairs of characters
// (s[0], s[l]), (s[l], s[2l]), (s[2l], s[3l]), ... in turn. For each such pair
// (s[i], s[j]) we will try to find the maximum number of times that a substring
// of length l repeats starting from any of the positions s[i], s[i - 1], ...,
// s[i - l + 1]. By taking the max over all such pairs over all possible values
// of `l`, we get the answer to the original problem. There are a total of n +
// n/2 + n/3 + ...  + n/n pairs in the string (roughly n log n).
//
// For a substring starting at s[i], the number of times it repeats starting
// from that position is equal to L/l + 1 where L is the longest common prefix
// of the suffixes s[i...] and s[j...]. However it may be that s[i] is not at
// the beginning of a substring that repeats the maximum number of times, and we
// have to start somewhere before s[i]. To detect this possibility we consider
// r = L % l. If r > 0, then we might possibly get a more optimal substring by
// starting from s[i - (l - r)]. For example:
//
// b a b a a b a a b a b
//   - - - - - - - - -
// 0 1 2 3 4 5 6 7 8 9 10
//
// Here, when i = 3 and j = 6 (l = 3), we have L = 4 and r = 1, which is an
// indication that the trailing r characters at the end of s[j ... j + L - 1]
// might form an additional repeat when combined with l - r characters before
// s[i] (namely, 2). So we then look at the LCP of the suffixes starting at
// positions 3 - 2 and 6 - 2, which is 6. This gives us the optimal number of
// repeats, 6 / 3 + 1 = 3.
//
// In order to efficiently answer LCP queries, we must compute the LCP array and
// then preprocess it for RMQ. This code uses Brian's implementation of the DC3
// algorithm (Kärkkäinen and Sanders) and the classic sparse table technique for
// RMQ that uses O(n log n) space and O(log n) time per query, for an overall
// time complexity of O(n log^2 n). It is possible to improve this complexity
// by doing RMQ in O(1) per query (assuming a word size of log n) (left as
// exercise for reader).

#include <algorithm>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;

template <class It, class F>
void radix_pass(It begin, It end, It out, int K, F f) {
    vector<int> cnt(K);
    for (It i = begin; i != end; i++) {
        cnt[f(*i)]++;
    }
    vector<int> pos(K);
    for (int i = 1; i < K; i++) {
        pos[i] = pos[i - 1] + cnt[i - 1];
    }
    for (It i = begin; i != end; i++) {
        int digit = f(*i);
        out[pos[digit]++] = *i;
    }
}

template <class It>
vector<int> suffix_array(It begin, It end, int K) {
    const auto n = end - begin;
    const auto m = 3 * ((n + 2) / 3);
    // step 1: compute suffix array of suffixes with i \nequiv 0 \pmod 3
    vector<int> skew_indices;
    for (int i = 0; i < m / 3; i++) {
        skew_indices.push_back(3 * i + 1);
        skew_indices.push_back(3 * i + 2);
    }
    // radix sort the skew indices
    vector<int> tmp(skew_indices.size());
    for (int i = 2; i >= 0; i--) {
        radix_pass(skew_indices.begin(), skew_indices.end(),
                   tmp.begin(),
                   K,
                   [&] (int j) {
                       return i + j < n ? begin[i + j] : 0;
                   });
        swap(skew_indices, tmp);
    }
    vector<int> rank_(m);
    int rank_cnt = 0;
    for (int i = 0; i < skew_indices.size(); i++) {
        if (i == 0 ||
            skew_indices[i] + 2 >= n ||
            skew_indices[i - 1] + 2 >= n ||
            begin[skew_indices[i]] != begin[skew_indices[i - 1]] ||
            begin[skew_indices[i] + 1] != begin[skew_indices[i - 1] + 1] ||
            begin[skew_indices[i] + 2] != begin[skew_indices[i - 1] + 2]) {
            ++rank_cnt;
        }
        rank_[skew_indices[i]] = rank_cnt;
    }
    vector<int> SA12;
    if (rank_cnt < skew_indices.size()) {
        vector<int> s12;
        for (int i = 1; i < m; i += 3) {
            s12.push_back(rank_[i]);
        }
        for (int i = 2; i < m; i += 3) {
            s12.push_back(rank_[i]);
        }
        SA12 = suffix_array(s12.begin(), s12.end(), rank_cnt + 1);
        for (int& x : SA12) {
            if (x < m / 3) {
                x = 3*x + 1;
            } else {
                x = 3*(x - m / 3) + 2;
            }
        }
        for (int i = 0; i < SA12.size(); i++) {
            rank_[SA12[i]] = i + 1;
        }
    } else {
        SA12 = move(skew_indices);
    }
    // step 2: compute suffix array of suffixes with i \equiv 0 \pmod 3
    vector<int> SA3;
    for (int i = 0; i < m; i += 3) {
        SA3.push_back(i);
    }
    tmp.resize(SA3.size());
    radix_pass(SA3.begin(), SA3.end(), tmp.begin(), SA12.size() + 1,
               [&] (int i) {
                   return i + 1 >= n ? 0 : rank_[i + 1];
               });
    swap(tmp, SA3);
    radix_pass(SA3.begin(), SA3.end(), tmp.begin(), K,
               [&] (int i) { return i >= n ? 0 : begin[i]; });
    swap(tmp, SA3);
    // step 3: merge
    vector<int> result;
    int i = 0, j = 0;
    while (i < SA3.size() && j < SA12.size()) {
        // compare suffixes
        int k1 = SA3[i];
        int k2 = SA12[j];
        bool c;
        int r1 = k1 < n ? begin[k1] : 0;
        int r2 = k2 < n ? begin[k2] : 0;
        if (r1 < r2) {
            c = true;
        } else if (r1 > r2) {
            c = false;
        } else if (k2 % 3 == 1) {
            r1 = k1 + 1 >= n ? 0 : rank_[k1 + 1];
            r2 = k2 + 1 >= n ? 0 : rank_[k2 + 1];
            c = r1 < r2;
        } else {
            r1 = k1 + 1 >= n ? 0 : begin[k1 + 1];
            r2 = k2 + 1 >= n ? 0 : begin[k2 + 1];
            if (r1 < r2) {
                c = true;
            } else if (r1 > r2) {
                c = false;
            } else {
                r1 = k1 + 2 >= n ? 0 : rank_[k1 + 2];
                r2 = k2 + 2 >= n ? 0 : rank_[k2 + 2];
                c = r1 < r2;
            }
        }
        int val;
        if (c) {
            val = SA3[i++];
        } else {
            val = SA12[j++];
        }
        if (val < n) result.push_back(val);
    }
    while (i < SA3.size()) {
        const int val = SA3[i++];
        if (val < n) result.push_back(val);
    }
    while (j < SA12.size()) {
        const int val = SA12[j++];
        if (val < n) result.push_back(val);
    }
    return result;
}

constexpr int max_len = 55555;

int buf[max_len];
int lcp[max_len];
int rank_[max_len];
int rm[max_len][16];
int lg2(int x) {
    return 31 - __builtin_clz(x);
}
int rmq(int a, int b) {
    int result = 1e6;
    while (a < b) {
        const int e = lg2(b - a);
        result = min(result, rm[a][e]);
        a += (1 << e);
    }
    return result;
}
int main() {
    int H; scanf("%d", &H);
    while (H--) {
        int n; scanf("%d", &n);
        if (n == 0) {
            printf("0\n");
            continue;
        }
        for (int i = 0; i < n; i++) {
            do {
                buf[i] = getchar();
            } while (!isalpha(buf[i]));
            buf[i] = buf[i] - 'a' + 1;
        }
        const auto sa = suffix_array(buf, buf + n, 3);
        for (int i = 0; i < n; ++i) {
            rank_[sa[i]] = i;
        }
        int k = 0;
        for (int i = 0; i < n; ++i) {
            if (k > 0) --k;
            if (rank_[i] == n - 1) {
                lcp[rank_[i]] = k = 0;
                continue;
            }
            int j = sa[rank_[i] + 1];
            while (i + k < n && j + k < n && buf[i + k] == buf[j + k]) ++k;
            lcp[rank_[i]] = k;
        }
        // prepare for rmq
        for (int i = 0; i < n; i++) {
            rm[i][0] = lcp[i];
        }
        for (int e = 1; (1 << e) <= n; e++) {
            for (int i = 0; i + (1 << e) <= n; i++) {
                rm[i][e] = min(rm[i][e - 1], rm[i + (1 << (e - 1))][e - 1]);
            }
        }
        // iterate over possible lengths of the repeated string
        int best = 1;
        for (int l = 1; l <= n / 2; l++) {
            for (int i = 0; i + l < n; i += l) {
                int a = rank_[i];
                int b = rank_[i + l];
                if (a > b) swap(a, b);
                int this_lcp = rmq(a, b);
                if (i > 0 && this_lcp % l > 0) {
                    a = rank_[i - (l - this_lcp % l)];
                    b = rank_[i + l - (l - this_lcp % l)];
                    if (a > b) swap(a, b);
                    this_lcp = max(this_lcp, rmq(a, b));
                }
                best = max(best, this_lcp / l + 1);
            }
        }
        printf("%d\n", best);
    }
    return 0;
}

