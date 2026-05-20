// 2026-05-20
// A greedy algorithm gives the correct answer: for each line, find the first
// occurrence of each emoji, and delete the leftmost character that is the last
// character of some emoji.  Then, continue with the portion of the string to
// the right of the deleted character, until there are no matches left.  They
// don't say how large the test file is, so it's hard to tell in advance whether
// this is fast enough.  Turns out it isn't (on SPOJ, anyway).  The obvious
// optimal approach is to use the Aho--Corasick algorithm, but I'm too lazy, so
// here's a solution that's slightly faster than the naive one and does pass:
// for each line, find all occurrences of each emoji, then use a priority queue
// to repeatedly select the one with the leftmost last character.
#include <queue>
#include <stdio.h>
#include <string>
#include <tuple>
#include <vector>
using namespace std;
struct Entry {
    int end;
    int word;
    int index;
};
bool operator<(const Entry& e1, const Entry& e2) {
    return e1.end > e2.end;
}
int do_testcase(int E, int L) {
    vector<string> emoji;
    while (E--) {
        int c;
        do {
            c = getchar_unlocked();
        } while (c == '\n');
        string s;
        do {
            s.push_back(c);
            c = getchar_unlocked();
        }
        while (c != '\n');
        emoji.push_back(s);
    }
    int result = 0;
    while (L--) {
        string line;
        // careful: lines may be empty
        int c;
        do {
            c = getchar_unlocked();
            line.push_back(c);
        } while (c != '\n');
        int i = 0;
        vector<vector<int>> matches(emoji.size());
        priority_queue<Entry> H;
        for (int i = 0; i < emoji.size(); i++) {
            for (int j = 0; j + emoji[i].size() <= line.size(); j++) {
                if (line.substr(j, emoji[i].size()) == emoji[i]) {
                    matches[i].push_back(j + emoji[i].size());
                }
            }
            if (!matches[i].empty()) {
                H.push({matches[i][0], i, 0});
            }
        }
        int cur = 0;
        while (cur < line.size() && !H.empty()) {
            ++result;
            int next = H.top().end;
            while (!H.empty()) {
                const auto& e = H.top();
                if (e.end - emoji[e.word].size() < next) {
                    if (e.index + 1 < matches[e.word].size()) {
                        H.push({matches[e.word][e.index + 1],
                                e.word,
                                e.index + 1});
                    }
                    H.pop();
                } else break;
            }
            cur = next;
        }
    }
    return result;
}
int main() {
    for (;;) {
        int E, L; scanf("%d %d", &E, &L); if (E + L == 0) break;
        printf("%d\n", do_testcase(E, L));
    }
}
