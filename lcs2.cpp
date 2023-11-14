// 2023-11-13
// Brian's implementation of the DC3 algorithm (Kärkkäinen and Sanders).
#include <algorithm>
#include <deque>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <utility>
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

constexpr int max_len = 1111111;

char s[max_len];
int buf[max_len];
int lcp[max_len];
int rank_[max_len];
int where[max_len];
int main() {
    int N = 0;
    int nstr = 0;
    for (;;) {
        if (!fgets(s, max_len, stdin)) break;
        if (s[0] == '\n') {
            // empty string
            puts("0");
            return 0;
        }
        if (nstr > 0) {
            where[N] = -1;
            buf[N++] = nstr;
        }
        for (const char* p = s; *p != '\n'; ++p) {
            where[N] = nstr;
            buf[N++] = *p;
        }
        ++nstr;
    }
    if (nstr == 1) {
        printf("%d\n", N);
        return 0;
    }
    vector<int> sa = suffix_array(buf, buf + N, 128);
    for (int i = 0; i < N; i++) {
        rank_[sa[i]] = i;
    }
    int k = 0;
    for (int i = 0; i < N; i++) {
        if (k > 0) --k;
        if (rank_[i] == N - 1) {
            lcp[rank_[i]] = k = 0;
            continue;
        }
        int j = sa[rank_[i] + 1];
        while (i + k < N && j + k < N && buf[i + k] == buf[j + k]) ++k;
        lcp[rank_[i]] = k;
    }
    int res = 0;
    // we have to do sliding window min here, unlike the 2 string version where
    // we just look for 2 adjacent suffixes from different strings
    int cnt[10] = {0};
    int nhave = 0;
    std::deque<std::pair<int, int>> Q;
    int j = 0;
    for (int i = 0; i < N; i++) {
        if (i > 0) {
            if (where[sa[i - 1]] >= 0) {
                if (0 == --cnt[where[sa[i - 1]]]) {
                    --nhave;
                }
            }
            if (Q.size() > 0 && Q.back().first == i - 1) {
                Q.pop_back();
            }
        }
        while (nhave < nstr && j < N) {
            if (where[sa[j]] >= 0) {
                if (0 == cnt[where[sa[j]]]++) {
                    ++nhave;
                }
            }
            if (j > i) {
                int val = lcp[j - 1];
                while (Q.size() > 0 && Q.front().second >= val) {
                    Q.pop_front();
                }
                Q.emplace_front(j - 1, val);
            }
            ++j;
        }
        if (nhave == nstr) {
            res = std::max(res, Q.back().second);
        }
    }
    printf("%d\n",res);
    return 0;
}
