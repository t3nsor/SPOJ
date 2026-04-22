// 2026-04-22
#include <algorithm>
#include <ctype.h>
#include <iostream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
using namespace std;
enum {
    LEFT,
    RIGHT,
    UP,
    DOWN,
};
// in the same order as the enum above
constexpr int dr[4] = {0, 0, -1, 1};
constexpr int dc[4] = {-1, 1, 0, 0};
struct Gate {
    char kind;
    int need;
    vector<int> inr;
    vector<int> inc;
    vector<int> outr;
    vector<int> outc;
};
void do_testcase(vector<string>& board) {
    const int R = board.size();
    int C = -1;
    for (int i = 0; i < R; i++) {
        C = max(C, (int)board[i].size());
    }
    for (int i = 0; i < R; i++) {
        while (board[i].size() < C) board[i].push_back(' ');
    }
    // for a '#' character belonging to a gate, this is the gate ID;
    // for any other character, it's the value that propagates to that location
    vector<vector<int>> val(R, vector<int>(C, -1));

    int outval[26];  // values of named outputs
    for (int i = 0; i < 26; i++) outval[i] = -1;

    // find all gates
    vector<Gate> gates;
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if (board[i][j] == '#') {
                if (val[i][j] >= 0) continue;
                int gate_id = gates.size();
                gates.emplace_back();
                Gate& gate = gates.back();
                int i2 = i;
                while (i2 < R && board[i2][j] == '#') {
                    val[i2][j] = gate_id;
                    if (j > 0 && board[i2][j - 1] == '=') {
                        gate.inr.push_back(i2);
                        gate.inc.push_back(j - 1);
                        gate.need++;
                    }
                    i2++;
                }
                int j2 = j;
                while (j2 < C && board[i][j2] == '#') {
                    val[i][j2] = gate_id;
                    j2++;
                }
                for (int k = i; k < i2; k++) {
                    val[k][j2 - 1] = gate_id;
                    if (j2 < C &&
                        (board[k][j2] == '=' || board[k][j2] == 'o')) {
                        gate.outr.push_back(k);
                        gate.outc.push_back(j2);
                    }
                }
                for (int k = j; k < j2; k++) val[i2 - 1][k] = gate_id;
                for (int k = i + 1; k < i2 - 1; k++) {
                    for (int m = j + 1; m < j2 - 1; m++) {
                        if (board[k][m] != ' ') {
                            gate.kind = board[k][m];
                            board[k][m] = ' ';  // so it doesn't look like an
                                                // input in the next step
                        }
                    }
                }
            }
        }
    }

    // find all inputs
    stack<pair<int, int>> S;
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if (isdigit(board[i][j])) {
                val[i][j] = board[i][j] - '0';
                S.emplace(i, j);
            }
        }
    }

    // handle gates with 0 inputs (maybe these exist?)
    for (const auto& g : gates) {
        if (g.need == 0) {
            const int result = (g.kind == '&' ? 1 : 0);
            for (int i = 0; i < g.outr.size(); i++) {
                val[g.outr[i]][g.outc[i]] = result;
                S.emplace(g.outr[i], g.outc[i]);
            }
        }
    }

    while (!S.empty()) {
        auto sq = S.top();
        S.pop();
        const int r = sq.first;
        const int c = sq.second;
        if (isupper(board[r][c])) {
            outval[board[r][c] - 'A'] = val[r][c];
            continue;
        }
        if (board[r][c] == 'o') {
            val[r][c + 1] = !val[r][c];
            S.emplace(r, c + 1);
            continue;
        }
        if (board[r][c] == '=' && board[r][c + 1] == '#') {
            // check for gate sufficiency
            Gate& g = gates[val[r][c + 1]];
            if (0 == --g.need) {
                int result = (g.kind == '&' ? 1 : 0);
                for (int i = 0; i < g.inr.size(); i++) {
                    int cur = val[g.inr[i]][g.inc[i]];
                    if (g.kind == '1') {
                        result |= cur;
                    } else if (g.kind == '&') {
                        result &= cur;
                    } else {
                        result ^= cur;
                    }
                }
                for (int i = 0; i < g.outr.size(); i++) {
                    val[g.outr[i]][g.outc[i]] = result;
                    S.emplace(g.outr[i], g.outc[i]);
                }
            }
            continue;
        }
        // otherwise check where we can send this value to
        for (int i = 0; i < 4; i++) {
            if (board[r][c] == '=') {
                if (i != RIGHT) continue;
            } else if (board[r][c] == '|') {
                if (i != UP && i != DOWN) continue;
            } else if (board[r][c] == '-') {
                if (i != LEFT && i != RIGHT) continue;
            }
            int r2 = r + dr[i];
            int c2 = c + dc[i];
            // pass through an arbitrary number of x characters (these are never
            // given a value)
            while (r2 >= 0 && r2 < R && c2 >= 0 && c2 < C &&
                   board[r2][c2] == 'x') {
                r2 += dr[i];
                c2 += dc[i];
            }
            if (r2 < 0 || r2 >= R || c2 < 0 || c2 >= C) {
                continue;
            }
            if (val[r2][c2] >= 0 || board[r2][c2] == ' ') continue;
            // enforce restrictions on what '+' characters connect to
            if (board[r][c] == '+') {
                if (board[r2][c2] == '=') {
                    if (i != RIGHT) continue;
                } else if (board[r2][c2] == '|') {
                    if (i != UP && i != DOWN) continue;
                } else if (board[r2][c2] == '-') {
                    if (i != LEFT && i != RIGHT) continue;
                }
            }
            val[r2][c2] = val[r][c];
            S.emplace(r2, c2);
        }
    }

    for (int i = 0; i < 26; i++) {
        if (outval[i] >= 0) cout << char(i + 'A') << '=' << outval[i] <<'\n';
    }
}
int main() {
    for (;;) {
        vector<string> board;
        for (;;) {
            string s; getline(cin, s);
            if (!s.empty() && s.back() == '\r') s.pop_back();
            if (!s.empty() &&
                all_of(s.begin(), s.end(), [](int c) { return c == '*'; })) {
                break;
            }
            board.push_back(move(s));
        }
        if (board.empty()) break;
        do_testcase(board);
        cout << '\n';
    }
}
