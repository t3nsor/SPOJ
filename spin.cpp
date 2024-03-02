// 2024-03-02
// Some terminology:
// * Z(N) is the string with length N consisting of all zeroes
// * H(N) is the string with length N where bit 0 is set and all other bits are
//   clear
//
// We can reformulate the rules of the game in terms of the representation in
// the input:
// * The state is a bit string S of length N together with a position x
//   (0 <= x <= N) with N denoting the leftmost possible position
// * In position x >= 1, only bit x - 1 can be toggled, subject to the further
//   restriction that, when x < N, bit x - 1 can be toggled only when bit x is
//   set (representing the vertical state). This counts as a move.
// * The value of x can be increased by 1 if the resulting value wouldn't exceed
//   N. The value of x can be decreased by 1 if bits x, x + 1, ..., N - 1 are
//   clear and the resulting value is non-negative. Increasing or decreasing x
//   by 1 counts as a move.
// * The win position is S = Z(N) and x = 0. (This differs by one move from the
//   win position defined in the problem statement, so we will just have to add
//   1 to whatever value we compute).
//
// Now the problem is how to calculate the distance d(P, Q) where P = (S, x)
// is the initial position, and Q = (T, 0) where T is either Z(N) or H(N).
// This can be done using the following recursive relations:
// * If S[0] = T[0], then we won't change S[0] again and can ignore it.
//   Therefore we shrink the size of the problem by 1, namely
//     d = d((S[1, ...], x - 1), (Z(N - 1), 0)) + 1
//   Note that when the smaller puzzle is in state (Z(N - 1), 0) it corresponds
//   to state (T, 1) in the original puzzle, which accounts for the increment at
//   the end.
// * Otherwise, we must clear bit 0, which is possible only when we are in the
//   state (?1000..., 1). The cost to reach state (?1000..., 1) can again be
//   done by ignoring the first bit, i.e.,
//     d = a + b where a = d((S[1, ...], x - 1), (H(N - 1), 0)) and
//                     b = d((?1000..., 1), Q)
//                       = 1 + d((H(N - 1), 0), (Z(N - 1), 0)) + 1
//   The additional + 1 in this case represents the move required to flip S[0].

#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;
int d(int N, int S, int x, int Q0) {
    if (N == 1) {
        if (S == Q0) {
            return x;
        } else {
            return 3 - x;
        }
    }
    static unordered_map<int, int> memo[2][32][32];
    auto& M = memo[Q0][N][x];
    if (M.count(S)) {
        return M[S];
    }
    auto& result = M[S];
    if ((S & 1) == Q0) {
        result = 1 + d(N - 1, S >> 1, x - 1, 0);
    } else {
        result = 2 + d(N - 1, S >> 1, x - 1, 1) + d(N - 1, 1, 0, 0);
    }
    return result;
}
void do_testcase() {
    int N; cin >> N;
    string s; cin >> s;
    int S = 0;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == 'v') {
            S |= (1 << i);
        }
    }
    int x; cin >> x;
    cout << 1 + d(N, S, x, 0) << '\n';
}
int main() {
    int T; cin >> T;
    while (T--) do_testcase();
}
