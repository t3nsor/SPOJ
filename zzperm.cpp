// 2024-04-15
// This is just DP where the state is how many copies of each letter we have
// left to use, the last letter we used, and whether the next letter must be
// larger or smaller than the last one. This approach doesn't run in time for
// the most difficult test cases (see ZZPERM2, which I have no idea how to
// solve) but it does get AC for this problem.
#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;
struct State {
    unsigned long long count;
    int last;
    int direction;  // 0 means next letter must be larger, 1 means smaller
    friend bool operator==(State s1, State s2) {
        return s1.count == s2.count &&
               s1.last == s2.last &&
               s1.direction == s2.direction;
    }
};
namespace std {
template <>
struct hash<State> {
    size_t operator()(State s) const {
        string str(reinterpret_cast<char*>(&s), sizeof(s));
        return hash<string>{}(str);
    }
};
}
struct Solution {
    string s;
    int D;
    unordered_map<State, int> memo = {};
    int calc(State state) {
        if (state.count == 0) return 1;
        auto it = memo.find(state);
        if (it != memo.end()) return it->second;
        int& result = memo[state];
        result = 0;
        if (state.direction == 0) {
            for (int i = state.last + 1; i < 10; i++) {
                if (0 == (state.count & (63ULL << (6*i)))) continue;
                result += calc({state.count - (1ULL << (6*i)), i, 1});
            }
        } else {
            for (int i = state.last - 1; i >= 0; i--) {
                if (0 == (state.count & (63ULL << (6*i)))) continue;
                result += calc({state.count - (1ULL << (6*i)), i, 0});
            }
        }
        return result;
    }
    void run() {
        unsigned long long count = 0;
        bool have[10] = {0};
        for (const char c : s) {
            have[c - 'a'] = true;
            count += 1ULL << (6*(c - 'a'));
        }
        if (count == 0) {
            // 64 of the same letter
            cout << "0\n";
            return;
        }
        int total = 0;
        int startswithcnt[10] = {0};
        for (int i = 0; i < 10; i++) {
            if (!have[i]) continue;
            State state = {count - (1ULL << (6*i)), i, 0};
            startswithcnt[i] += calc(state);
            if (state.count > 0) {
                state.direction = 1;
                startswithcnt[i] += calc(state);
            }
            total += startswithcnt[i];
        }
        const int n = total / D;
        for (int i = 0; i < n; i++) {
            string word;
            int index = D*i - 1 + D;
            int j = 0;
            for (;; j++) {
                if (index >= startswithcnt[j]) {
                    index -= startswithcnt[j];
                } else {
                    word.push_back('a' + j);
                    break;
                }
            }
            State state = {count - (1ULL << (6*j)), j, -1};
            while (word.size() < s.size()) {
                for (int j = 0; j < 10; j++) {
                    if (j == word.back() - 'a') continue;
                    if (0 == (state.count & (63ULL << (6*j)))) continue;
                    const int nextdir = j > word.back() - 'a';
                    if (state.direction == nextdir) continue;
                    int c = calc({state.count - (1ULL << (6*j)),
                                  j,
                                  nextdir});
                    if (index >= c) {
                        index -= c;
                    } else {
                        state.count -= (1ULL << (6*j));
                        word.push_back('a' + j);
                        state.last = j;
                        state.direction = nextdir;
                        break;
                    }
                }
            }
            cout << word << '\n';
        }
        cout << total << "\n\n";
    }
};
int main() {
    string s;
    int D;
    while (cin >> s >> D) {
        Solution{s, D}.run();
    }
}
