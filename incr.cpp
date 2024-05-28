// 2024-05-28
// Suppose we've already used some of the numbers in {1, ..., N} to form a
// prefix. The relevant information about this state is:
// 1. how many unused numbers are there such that, if we chose such a number as
//    the next number in the prefix, the longest increasing subsequence ending
//    at that number would have length 1
// 2-5. same, but replace "1" with 2-5
// The initial state is {N, 0, 0, 0, 0}. The final state is {0, 0, 0, 0, 0}. The
// primary form of transition involves selecting a nonempty bucket (i.e. a
// nonzero value from 1-5 above) and selecting a rank from that bucket. For
// example we can transition from {1, 5, 2, 0, 0} by choosing the second largest
// number in bucket 2 (and appending it to each prefix represented by this
// state). This causes the former largest number in bucket 2 to move to bucket
// 3 (since the just-chosen number has an IS of length 2 ending at it, and this
// larger number would thus have an IS of length 3). The resulting state is
// therefore {1, 3, 3, 0, 0}.
//
// The main problem with this approach is that the state itself doesn't encode
// the length of the LIS; for example in {1, 5, 2, 0, 0} you know the length of
// the LIS is at least 1 since there are 2 ways to generate an IS of length 2,
// but the final state {0, 0, 0, 0, 0} represents all LIS lengths. To avoid
// having to introduce a sixth datum to the state (the length of the LIS so far)
// I run the algorithm in phases: at the beginning of phase k we know the count
// of each state with the constraint that the length of the LIS so far is
// exactly k-1. This is really slow and barely passes in time (time limit =
// 5.3s, my time = 5.1s) so maybe it wasn't the best strategy, but I don't want
// to spend any more time optimizing it :|
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;
constexpr int MOD = 1000000000;
int get_state(int state, int index) {
    return 63 & (state >> (6 * index));
}
int inc_state(int state, int index, int inc) {
    return state + (inc << (6 * index));
}
int sum(int state) {
    return (state & 63) + ((state >> 6) & 63) + ((state >> 12) & 63) +
           ((state >> 18) & 63) + (state >> 24);
}
void do_testcase() {
    int N, B; cin >> N >> B;
    unordered_map<int, int> M;
    M[N] = 1;
    for (int l = 1; l <= B; l++) {
        unordered_map<int, int> M2;
        vector<queue<int>> queues(N + 1);
        int current_sum = N;
        for (const auto entry : M) {
            const int s = entry.first;
            const int c = get_state(s, l - 1);
            for (int i = 0; i < c; i++) {
                int s2 = inc_state(s, l - 1, i - c);
                if (l < B) {
                    s2 = inc_state(s2, l, c - i - 1);
                } else {
                    if (i != c - 1) continue;
                }
                auto it = M2.find(s2);
                if (it == M2.end()) {
                    queues[sum(s2)].push(s2);
                    M2[s2] = entry.second;
                } else {
                    it->second = (it->second + entry.second) % MOD;
                }
            }
        }
        M = move(M2);
        while (current_sum > 0) {
            while (!queues[current_sum].empty()) {
                const int s = queues[current_sum].front();
                queues[current_sum].pop();
                // do not increase the current max length
                for (int k = 0; k < l; k++) {
                    int c = get_state(s, k);
                    for (int i = 0; i < c; i++) {
                        int s2 = inc_state(s, k, i - c);
                        if (k + 1 < B) {
                            s2 = inc_state(s2, k + 1, c - i - 1);
                        } else {
                            if (i != c - 1) continue;
                        }
                        auto it = M.find(s2);
                        if (it == M.end()) {
                            queues[sum(s2)].push(s2);
                            M[s2] = M[s];
                        } else {
                            it->second = (it->second + M[s]) % MOD;
                        }
                    }
                }
            }
            --current_sum;
        }
    }
    cout << M[0] << '\n';
}
int main() {
    int T; cin >> T;
    while (T--) do_testcase();
}
