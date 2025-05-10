// 2025-05-09
// The only optimizations we use here are
// - deduplicate rotations/reflections of each shape
// - fill up the squares in left-to-right, top-to-bottom order so if we are
//   thinking about placing a piece in a given square, we know which bead in the
//   piece goes in that square: the leftmost one in the topmost row.
// This passes on SPOJ (AC in 4.5s, TL = 5s) but gets TLE on DMOJ.
#include <algorithm>
#include <string>
#include <utility>
#include <vector>
using namespace std;

vector<string> rawdata[12] = {
    {"..",
     ". "},

    {"...."},

    {"...", 
     ".  "},

    {"..",
     ".."},

    {".  ",
     ".  ",
     "..."},

    {"....",
     " .  "},

    {"...",
     ". ."},

    {"...",
     ".. "},

    {"... ",
     "  .."},

    {" . ",
     "...",
     " . "},

    {".  ",
     ".. ",
     " .."},

    {"....",
     ".   "}
};

using Option = vector<pair<int, int>>;

vector<vector<Option>> options(12);

void generate_options() {
    for (int i = 0; i < 12; i++) {
        const auto& shape = rawdata[i];
        vector<pair<int, int>> squares;
        for (int r = 0; r < shape.size(); r++) {
            for (int c = 0; c < shape[r].size(); c++) {
                if (shape[r][c] == '.') squares.emplace_back(r, c);
            }
        }
        vector<vector<pair<int, int>>> variations;
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 4; k++) {
                variations.push_back(squares);
                // rotate
                for (int p = 0; p < squares.size(); p++) {
                    const int r = squares[p].first;
                    const int c = squares[p].second;
                    squares[p] = make_pair(c, -r);
                }
            }
            // flip
            for (int k = 0; k < squares.size(); k++) {
                squares[k].first *= -1;
            }
        }
        // normalize
        for (auto& variation: variations) {
            sort(variation.begin(), variation.end());
            const int r0 = variation[0].first;
            const int c0 = variation[0].second;
            for (auto& p : variation) {
                p.first -= r0;
                p.second -= c0;
            }
        }
        // deduplicate
        sort(variations.begin(), variations.end());
        const auto uniqEnd = unique(variations.begin(), variations.end());
        for (auto it = variations.begin(); it != uniqEnd; it++) {
            options[i].push_back(*it);
        }
    }
}

bool in_bounds(int r, int c) {
    return r >= 0 && c >= 0 && r < 10 && c <= r;
}

bool rec(char (&grid)[10][11], bool (&avail)[12], int unfilled, int r, int c) {
    if (unfilled == 0) return true;
    if (c > r) return rec(grid, avail, unfilled, r + 1, 0);
    if (grid[r][c] != '.') return rec(grid, avail, unfilled, r, c + 1);
    for (int i = 0; i < 12; i++) {
        if (!avail[i]) continue;
        for (const auto& option : options[i]) {
            bool fail = false;
            for (int j = 0; j < option.size(); j++) {
                const int r2 = r + option[j].first;
                const int c2 = c + option[j].second;
                if (!in_bounds(r2, c2) || grid[r2][c2] != '.') {
                    fail = true;
                    break;
                }
            }
            if (fail) continue;
            for (int j = 0; j < option.size(); j++) {
                const int r2 = r + option[j].first;
                const int c2 = c + option[j].second;
                grid[r2][c2] = i + 'A';
            }
            avail[i] = false;
            unfilled -= option.size();
            if (rec(grid, avail, unfilled, r, c + 1)) return true;
            unfilled += option.size();
            avail[i] = true;
            for (int j = 0; j < option.size(); j++) {
                const int r2 = r + option[j].first;
                const int c2 = c + option[j].second;
                grid[r2][c2] = '.';
            }
        }
    }
    return false;
}

void do_testcase() {
    char grid[10][11] = {{0}};
    bool avail[12];
    fill(avail, avail + 12, true);
    int unfilled = 0;
    for (int r = 0; r < 10; r++) {
        for (int c = 0; c <= r; c++) {
            char ch; do { ch = getchar(); } while (ch <= 32);
            grid[r][c] = ch;
            if (ch == '.') {
                ++unfilled;
            } else {
                avail[ch - 'A'] = false;
            }
        }
    }
    if (rec(grid, avail, unfilled, 0, 0)) {
        for (int r = 0; r < 10; r++) puts(grid[r]);
    } else {
        puts("No solution");
    }
}

int main() {
    generate_options();
    for (int i = 0; i < 10; i++) do_testcase();
}
