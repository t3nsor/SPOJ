// 2025-07-10
// The stupid brute force approach works, thanks to compilers being good at
// optimization. On the sample data, this program is about 12 times faster with
// `-O2` than `-O0` (GCC 12). But there are much faster accepted times on SPOJ,
// presumably using recursive backtracking approaches that break early if a
// contradiction is found early. This is left as an exercise for the reader.
#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>
using namespace std;
vector<array<int, 9>> P;
void read_grid(char (&grid)[9][9]) {
    for (int i = 0; i < 9; i++) {
        string s; while (s.empty()) cin >> s;
        for (int j = 0; j < 9; j++) grid[i][j] = s[j] - '0';
    }
}
struct KnownCell {
    int r;
    int c;
    int val;
};
std::mt19937 rng(3749419463U);
bool do_testcase() {
    char solved[9][9], unsolved[9][9];
    read_grid(solved);
    read_grid(unsolved);
    vector<KnownCell> knownCells;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (unsolved[i][j]) {
                knownCells.push_back({i, j, unsolved[i][j]});
            }
        }
    }
    for (int o = 0; o < 4; o++) {
        // rotate known cells
        for (auto& cell : knownCells) {
            swap(cell.r, cell.c);
            cell.r = 8 - cell.r;
        }
        // randomly permute them (not needed to get AC)
        shuffle(knownCells.begin(), knownCells.end(), rng);
        for (int i = 0; i < P.size(); i++) {
            for (int j = 0; j < P.size(); j++) {
                int l[10] = {0};
                bool success = true;
                for (const auto& cell : knownCells) {
                    const int pr = P[i][cell.r];
                    const int pc = P[j][cell.c];
                    if (!l[cell.val]) {
                        l[cell.val] = solved[pr][pc];
                    } else if (l[cell.val] != solved[pr][pc]) {
                        success = false;
                        break;
                    }
                }
                if (success) return true;
            }
        }
    }
    return false;
}
int main() {
    array<int, 9> A;
    for (int i = 0; i < 9; i++) A[i] = i;
    do {
        if (A[0] / 3 == A[1] / 3 && A[0] / 3 == A[2] / 3 &&
            A[3] / 3 == A[4] / 3 && A[3] / 3 == A[5] / 3 &&
            A[6] / 3 == A[7] / 3 && A[6] / 3 == A[8] / 3) {
            P.push_back(A);
        }
    } while (next_permutation(A.begin(), A.end()));
    ios::sync_with_stdio(false);
    int N; cin >> N;
    while (N--) {
        cout << (do_testcase() ? "Yes" : "No") << '\n';
    }
}
