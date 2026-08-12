// 2026-08-11
// Faces are assigned as follows (as in the problem statement but shifted by 1):
//
//   0
// 1 2 3 4
//   5
//
// Sides are assigned as follows:
// +---0---+
// |       |
// 3       1
// |       |
// +---2---+
//
// Similarly for the corner view:
//       +
//     /   \
//    /1   2\
//   /       \
//  +    0    +
//  |\       /|
//  | \0   3/ |
// 2| 3\   /0 |1
//  |    +    |
//  +    |    +  <--- 1
//   \  0|3  /
//    1  |  2
//  ^  \ | /
//  |    +
//  2
//
// Now we can create a table of possible corner views.  They are each described
// by 6 integers: the IDs of the face from the net that are visible at positions
// 0, 1, and 2 in the corner view, respectively, and for each such visible face,
// the edge that is visible at the position 0 in the corner view.
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
int pretable[8][6] = {
  {0, 2, 1, 3, 0, 1},
  {0, 3, 2, 2, 0, 1},
  {0, 4, 3, 1, 0, 1},
  {0, 1, 4, 0, 0, 1},
  {1, 2, 5, 2, 3, 0},
  {1, 5, 4, 3, 3, 2},
  {2, 3, 5, 2, 3, 1},
  {3, 4, 5, 2, 3, 2}
};
// A rotated version of a corner view can be obtained by applying the same
// 3-cycle to the first and second half of a 6-tuple.
int table[24][6];
void init_table() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 6; j++) {
            table[3*i][j] = pretable[i][j];
            table[3*i + 1][j] = pretable[i][j];
            table[3*i + 2][j] = pretable[i][j];
        }
        rotate(table[3*i + 1], table[3*i + 1] + 1, table[3*i + 1] + 3);
        rotate(table[3*i + 1] + 3, table[3*i + 1] + 4, table[3*i + 1] + 6);
        rotate(table[3*i + 2], table[3*i + 2] + 2, table[3*i + 2] + 3);
        rotate(table[3*i + 2] + 3, table[3*i + 2] + 5, table[3*i + 2] + 6);
    }
}
bool is_match(const string& net, const string& view) {
    for (const auto& row : table) {
        bool valid = true;
        for (int vface = 0; vface < 3; vface++) {
            const int nface = row[vface];
            const int nofs = row[vface + 3];
            if (net[2*nface] != view[2*vface] ||
                net[2*nface+1] != (view[2*vface+1] + nofs) % 4) {
                valid = false;
            }
        }
        if (valid) return true;
    }
    return false;
}
void do_testcase() {
    string net; cin >> net;
    for (int i = 0; i < 6; i++) { net[2*i+1] -= '1'; net[2*i+1] %= 4; }
    string views[5];
    for (int i = 0; i < 5; i++) {
        cin >> views[i];
        for (int j = 0; j < 3; j++) {
            views[i][2*j+1] -= '1';
            views[i][2*j+1] %= 4;
        }
    }
    bool match[5];
    int nmatch = 0;
    for (int i = 0; i < 5; i++) {
        nmatch += (match[i] = is_match(net, views[i]));
    }
    cout << nmatch;
    for (int i = 0; i < 5; i++) {
        cout << ' ' << (match[i] ? 'Y' : 'N');
    }
    cout << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    init_table();
    int T; cin >> T;
    for (int _cs = 1; _cs <= T; _cs++) {
        cout << _cs << ' ';
        do_testcase();
    }
}
