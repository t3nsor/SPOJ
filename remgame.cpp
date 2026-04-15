// 2026-04-15
// Brute force is the way to do it!  The comments say the test data are weak; in
// any case it is a useful exercise to try to do some "easy" optimizations.  We
// precompute all possible moves that can be done to a given row (represented as
// a 5-bit pattern) and store the list of lists as a 2d array to ensure cache
// locality (unlike a vector of vectors).  When a board position is represented
// as a 25-bit integer, extracting and updating row values is easy.  A column is
// given by a set of 5 bits that are spaced out by 5 positions from each other;
// to extract a column we use masking and shifting, followed by a lookup table
// to convert a number of the form a + b*2^5 + c*2^10 + d*2^15 + e*2^20 to a
// row value (a + 2b + 4c + 8d + 16e), and a separate lookup table translates a
// "row" (that actually represents a column) into a list of possible *column*
// moves (this saves a bit of time by avoiding an additional lookup to convert
// the row move into a column move when we later do the actual recursion).
#include <iostream>
using namespace std;
int hmoves[32][16];
int vtoh[1 << 21];
int htov[32];
int vmoves[32][16];
void init() {
    int b[5];
    int row = 0;
    for (b[4] = 0; b[4] < 2; b[4]++) {
        for (b[3] = 0; b[3] < 2; b[3]++) {
            for (b[2] = 0; b[2] < 2; b[2]++) {
                for (b[1] = 0; b[1] < 2; b[1]++) {
                    for (b[0] = 0; b[0] < 2; b[0]++, row++) {
                        int* move = hmoves[row];
                        // find consecutive set bits
                        for (int i = 0; i < 5; i++) {
                            int sub = 0;
                            for (int j = i; j < 5 && b[j]; j++) {
                                sub += 1 << j;
                                *move++ = sub;
                            }
                        }
                        htov[row] = b[0] + (b[1] << 5) + (b[2] << 10) +
                                    (b[3] << 15) + (b[4] << 20);
                    }
                }
            }
        }
    }
    row = 0;
    for (b[4] = 0; b[4] < 2; b[4]++) {
        for (b[3] = 0; b[3] < 2; b[3]++) {
            for (b[2] = 0; b[2] < 2; b[2]++) {
                for (b[1] = 0; b[1] < 2; b[1]++) {
                    for (b[0] = 0; b[0] < 2; b[0]++, row++) {
                        int col = b[0] + (b[1] << 5) + (b[2] << 10) +
                                  (b[3] << 15) + (b[4] << 20);
                        vtoh[col] = row;
                        for (int *hmove = hmoves[row], *vmove = vmoves[row];
                             *hmove;
                             ++hmove, ++vmove) {
                            *vmove = htov[*hmove];
                        }
                    }
                }
            }
        }
    }
}
enum Winner {
    UNKNOWN,
    NEXT,
    PREV,
};
Winner memo[1 << 25];
void calc(int pos) {
    if (memo[pos] != UNKNOWN) return;
    if (pos == 0) { memo[pos] = NEXT; return; }
    for (int row = 0; row < 5; row++) {
        int val = 31 & (pos >> (5 * row));
        for (int* move = hmoves[val]; *move; ++move) {
            int newpos = pos - (*move << (5*row));
            calc(newpos);
            if (memo[newpos] == PREV) { memo[pos] = NEXT; return; }
        }
    }
    for (int col = 0; col < 5; col++) {
        constexpr int mask = 0x108421;  // 0b100001000010000100001
        int vval = mask & (pos >> col);
        int hval = vtoh[vval];
        for (int* move = vmoves[hval]; *move; ++move) {
            int newpos = pos - (*move << col);
            calc(newpos);
            if (memo[newpos] == PREV) { memo[pos] = NEXT; return; }
        }
    }
    memo[pos] = PREV;
}
void do_testcase() {
    int pos = 0;
    for (int i = 0; i < 25; i++) {
        int bit; cin >> bit; pos += bit * (1 << i);
    }
    calc(pos);
    cout << (memo[pos] == NEXT ? "winning" : "losing") << '\n';
}
int main() {
    init();
    int T; cin >> T; while (T--) do_testcase();
}
