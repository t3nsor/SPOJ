// 2024-02-26
// We use a pretty simple brute force approach: first generate all the possible
// ways in which numbers could be laid out considering one direction only (i.e.
// such that all numbers in a row are equal, but each number is used not more
// than 9 times) and then for each triple of such ways, check whether it leads
// to the 19 hexes on the board all being distinct. As an optimization, for the
// first direction only, we can use reflection symmetry to reduce the number of
// combinations, and also disregard any layouts in which a smaller number is
// used more times than a larger number. We precompute all layouts once on
// startup; it turns out there are only 216. Then, it's fast to process each
// test case. There's also supposedly some 3-line solution (according to the
// comments), which I guess is hardcoding some case analysis. This is left as an
// exercise for the reader (because I don't have time to figure it out).
#include <algorithm>
#include <array>
#include <iostream>
#include <set>
#include <utility>
#include <vector>
using namespace std;

vector<array<int, 5>> generate_first() {
    vector<array<int, 5>> result;
    array<int, 5> i;
    const int size[5] = {3, 4, 5, 4, 3};
    for (i[0] = 0; i[0] < 3; i[0]++) {
        for (i[1] = 0; i[1] < 3; i[1]++) {
            for (i[2] = 0; i[2] < 3; i[2]++) {
                for (i[3] = 0; i[3] < 3; i[3]++) {
                    for (i[4] = 0; i[4] < 3; i[4]++) {
                        int sum[3] = {0, 0, 0};
                        for (int j = 0; j < 5; j++) sum[i[j]] += size[j];
                        if (sum[0] > 9 || sum[1] > 9 || sum[2] > 9 ||
                            sum[0] > sum[1] || sum[1] > sum[2]) continue;
                        array<int, 5> ir = i;
                        reverse(ir.begin(), ir.end());
                        if (i <= ir) result.push_back(i);
                    }
                }
            }
        }
    }
    return result;
}

vector<array<int, 5>> generate_other() {
    vector<array<int, 5>> result;
    array<int, 5> i;
    const int size[5] = {3, 4, 5, 4, 3};
    for (i[0] = 0; i[0] < 3; i[0]++) {
        for (i[1] = 0; i[1] < 3; i[1]++) {
            for (i[2] = 0; i[2] < 3; i[2]++) {
                for (i[3] = 0; i[3] < 3; i[3]++) {
                    for (i[4] = 0; i[4] < 3; i[4]++) {
                        int sum[3] = {0, 0, 0};
                        for (int j = 0; j < 5; j++) sum[i[j]] += size[j];
                        if (sum[0] > 9 || sum[1] > 9 || sum[2] > 9) continue;
                        result.push_back(i);
                    }
                }
            }
        }
    }
    return result;
}

const vector<array<int, 5>> row_combinations[3] = {generate_first(),
                                                   generate_other(),
                                                   generate_other()};

// coordinates of the 19 hexes, in some arbitrary order, with some arbitrary
// assignment of axes
const int coordinates[19][3] = {
    {0, 2, 4},
    {0, 1, 3},
    {0, 0, 2},
    {1, 3, 4},
    {1, 2, 3},
    {1, 1, 2},
    {1, 0, 1},
    {2, 4, 4},
    {2, 3, 3},
    {2, 2, 2},
    {2, 1, 1},
    {2, 0, 0},
    {3, 4, 3},
    {3, 3, 2},
    {3, 2, 1},
    {3, 1, 0},
    {4, 4, 2},
    {4, 3, 1},
    {4, 2, 0}
};

vector<array<int, 3>> generate() {
    vector<array<int, 3>> result;
    array<int, 3> i;
    for (i[0] = 0; i[0] < row_combinations[0].size(); i[0]++) {
        for (i[1] = 0; i[1] < row_combinations[1].size(); i[1]++) {
            for (i[2] = 0; i[2] < row_combinations[2].size(); i[2]++) {
                // check for uniqueness
                set<array<int, 3>> s;
                for (int j = 0; j < 19; j++) {
                    array<int, 3> a;
                    for (int k = 0; k < 3; k++) {
                        const int coord = coordinates[j][k];
                        const int value = row_combinations[k][i[k]][coord];
                        a[k] = value;
                    }
                    s.insert(a);
                }
                if (s.size() == 19) {
                    result.push_back(i);
                }
            }
        }
    }
    return result;
}

const auto combinations = generate();

void do_testcase() {
    int value[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cin >> value[i][j];
        }
        sort(begin(value[i]), end(value[i]));
    }
    int best = 0;
    for (const auto& combination : combinations) {
        int sum = 0;
        for (int i = 0; i < 3; i++) {
            const auto& row_combination = row_combinations[i][combination[i]];
            for (int j = 0; j < 5; j++) {
                static const int size[5] = {3, 4, 5, 4, 3};
                sum += value[i][row_combination[j]] * size[j];
            }
        }
        if (sum > best) best = sum;
    }
    cout << best;
}

int main() {
    int T; cin >> T;
    for (int tc = 1; tc <= T; tc++) {
        cout << "Test #" << tc << '\n';
        do_testcase();
        cout << '\n';
    }
}
