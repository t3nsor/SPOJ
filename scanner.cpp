// 2024-03-09
// We use a relatively simple backtracking approach. Initially, all cells are
// unknown. At each step, if there is a scanline that already has the number of
// ones indicated in the input, set all cells in that scanline to 0. Otherwise,
// if there is a scanline for which the number of undetermined cells is exactly
// equal to the remaining number of ones that it needs, set all cells in that
// scanline to 1. Otherwise, choose a scanline that has the minimum number of
// possibilities (i.e. N choose K is minimal, where N is the number of
// undetermined cells in that scanline, and K is the remaining number of ones
// required in the scanline), pick a cell in that scanline, and recursively try
// setting it to 0 or 1.
#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
using namespace std;
int choose[25][25];
bool rec(int (&grid)[150], int remaining, set<int> (&members)[73],
         int (&need)[73], int& total_need, const vector<int> (&teams)[150]) {
    if (remaining == 0) return total_need == 0;
    int best_choose = 1e9;
    int best_index = -1;
    for (int i = 0; i < 73; i++) {
        if (members[i].empty()) continue;
        if (need[i] == 0) {
            vector<int> zero(members[i].begin(), members[i].end());
            bool fail = false;
            for (const int cell : zero) {
                --remaining;
                grid[cell] = 0;
                for (const int team : teams[cell]) {
                    members[team].erase(cell);
                    if (need[team] > members[team].size()) {
                        fail = true;
                    }
                }
            }
            if (!fail) {
                if (rec(grid, remaining, members, need, total_need, teams)) {
                    return true;
                }
            }
            for (const int cell : zero) {
                ++remaining;
                grid[cell] = -1;
                for (const int team : teams[cell]) {
                    members[team].insert(cell);
                }
            }
            return false;
        }
        if (need[i] == members[i].size()) {
            vector<int> one(members[i].begin(), members[i].end());
            bool fail = false;
            for (const int cell : one) {
                --remaining;
                grid[cell] = 1;
                for (const int team : teams[cell]) {
                    members[team].erase(cell);
                    need[team]--;
                    total_need--;
                    if (need[team] < 0) fail = true;
                }
            }
            if (!fail) {
                if (rec(grid, remaining, members, need, total_need, teams)) {
                    return true;
                }
            }
            for (const int cell : one) {
                ++remaining;
                grid[cell] = -1;
                for (const int team : teams[cell]) {
                    members[team].insert(cell);
                    need[team]++;
                    total_need++;
                }
            }
            return false;
        }
        int c = choose[members[i].size()][need[i]];
        if (c < best_choose) {
            best_choose = c;
            best_index = i;
        }
    }
    const int cell = *members[best_index].begin();
    grid[cell] = 0;
    --remaining;
    bool fail = false;
    for (const int team : teams[cell]) {
        members[team].erase(cell);
        if (need[team] > members[team].size()) {
            fail = true;
        }
    }
    if (!fail && rec(grid, remaining, members, need, total_need, teams)) {
        return true;
    }
    fail = false;
    grid[cell] = 1;
    for (const int team : teams[cell]) {
        need[team]--;
        if (need[team] < 0) {
            fail = true;
        }
        total_need--;
    }
    if (!fail && rec(grid, remaining, members, need, total_need, teams)) {
        return true;
    }
    grid[cell] = -1;
    ++remaining;
    for (const int team : teams[cell]) {
        members[team].insert(cell);
        need[team]++;
        total_need++;
    }
    return false;
}
void do_testcase() {
    set<int> members[73];
    int need[73];
    vector<int> teams[150];
    int grid[150];
    fill(grid, grid + 150, -1);
    for (int r = 0; r < 10; r++) {
        for (int c = 0; c < 15; c++) {
            members[r].insert(15*r + c);
            teams[15*r + c].push_back(r);
        }
    }
    for (int i = 0; i < 24; i++) {
        for (int r = 0; r < 10; r++) {
            for (int c = 0; c < 15; c++) {
                if (r + c == i) {
                    members[10 + i].insert(15*r + c);
                    teams[15*r + c].push_back(10 + i);
                }
            }
        }
    }
    for (int c = 0; c < 15; c++) {
        for (int r = 0; r < 10; r++) {
            members[34 + c].insert(15*r + c);
            teams[15*r + c].push_back(34 + c);
        }
    }
    for (int i = 0; i < 24; i++) {
        for (int r = 0; r < 10; r++) {
            for (int c = 0; c < 15; c++) {
                if (c - r + 9 == i) {
                    members[49 + i].insert(15*r + c);
                    teams[15*r + c].push_back(49 + i);
                }
            }
        }
    }
    int total_need = 0;
    for (int i = 0; i < 73; i++) {
        cin >> need[i];
        total_need += need[i];
    }
    rec(grid, 150, members, need, total_need, teams);
    for (int r = 0; r < 10; r++) {
        for (int c = 0; c < 15; c++) {
            if (grid[15*r + c] == 0) {
                cout << '.';
            } else if (grid[15*r + c] == 1) {
                cout << '#';
            } else {
                throw;
            }
        }
        cout << '\n';
    }
}
int main() {
    choose[0][0] = 1;
    for (int i = 1; i <= 24; i++) {
        choose[i][0] = choose[i][i] = 1;
        for (int j = 1; j < i; j++) {
            choose[i][j] = choose[i-1][j] + choose[i-1][j-1];
        }
    }
    int t; cin >> t;
    for (int cs = 0; cs < t; cs++) {
        do_testcase();
        if (cs < t - 1) {
            cout << '\n';
        }
    }
}
