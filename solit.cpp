// 2023-03-25
// Simple meet-in-the-middle based approach. An `unsigned long long` value is
// used as a 64-bit bitmask representing which squares are occupied. I also
// tried a non-meet-in-the-middle memoization-based approach, but it got TLE.
#include <iostream>
#include <unordered_set>
#include <utility>
using namespace std;
unsigned long long get_state(const pair<int, int> (&a)[4]) {
    return (1ULL << (8 * a[0].first + a[0].second)) |
           (1ULL << (8 * a[1].first + a[1].second)) |
           (1ULL << (8 * a[2].first + a[2].second)) |
           (1ULL << (8 * a[3].first + a[3].second));
}
bool has_piece(unsigned long long state, pair<int, int> rc) {
    return state & (1ULL << (8 * rc.first + rc.second));
}
constexpr int dr[4] = {1, 0, -1, 0};
constexpr int dc[4] = {0, 1, 0, -1};

enum Stage {
    GENERATE,
    CHECK
};

template <Stage stage>
bool rec(pair<int, int> (&cur)[4], int moves_left,
         unordered_set<unsigned long long>& S) {
    const auto cur_state = get_state(cur);
    if (stage == CHECK) {
        if (S.count(cur_state)) return true;
        if (moves_left == 0) return false;
    } else {
        S.insert(cur_state);
        if (moves_left == 0) return false;
    }
    for (int i = 0; i < 4; i++) {
        for (int dir = 0; dir < 4; dir++) {
            pair<int, int> new_pos = {cur[i].first + dr[dir],
                                      cur[i].second + dc[dir]};
            if (new_pos.first < 0 || new_pos.first >= 8 ||
                new_pos.second < 0 || new_pos.second >= 8) continue;
            if (has_piece(cur_state, new_pos)) {
                // try to jump over it
                new_pos.first += dr[dir];
                new_pos.second += dc[dir];
                if (new_pos.first < 0 || new_pos.first >= 8 ||
                    new_pos.second < 0 || new_pos.second >= 8 ||
                    has_piece(cur_state, new_pos)) continue;
            }
            swap(cur[i], new_pos);
            if (rec<stage>(cur, moves_left - 1, S)) return true;
            swap(new_pos, cur[i]);
        }
    }
    return false;
}

void do_testcase() {
    pair<int, int> a[4];
    pair<int, int> b[4];
    for (int i = 0; i < 4; i++) {
        cin >> a[i].first >> a[i].second;
        --a[i].first; --a[i].second;
    }
    for (int i = 0; i < 4; i++) {
        cin >> b[i].first >> b[i].second;
        --b[i].first; --b[i].second;
    }
    unordered_set<unsigned long long> S;
    rec<GENERATE>(a, 4, S);
    puts(rec<CHECK>(b, 4, S) ? "YES" : "NO");
}
int main() {
    int t; cin >> t;
    while (t--) do_testcase();
}
