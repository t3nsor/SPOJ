// 2023-10-14
#include <iostream>
using namespace std;
int rec(int (&a)[7][8], bool (&used)[7][7], int row, int col) {
    if (row == 7) return 1;
    if (col == 8) return rec(a, used, row + 1, 0);
    if (a[row][col] < 0) return rec(a, used, row, col + 1);
    int result = 0;
    if (col < 7 && a[row][col + 1] >= 0) {
        int x = a[row][col];
        int y = a[row][col + 1];
        if (!used[min(x, y)][max(x, y)]) {
            used[min(x, y)][max(x, y)] = true;
            a[row][col] = -1;
            a[row][col + 1] = -1;
            result += rec(a, used, row, col + 2);
            used[min(x, y)][max(x, y)] = false;
            a[row][col] = x;
            a[row][col + 1] = y;
        }
    }
    if (row < 6 && a[row + 1][col] >= 0) {
        int x = a[row][col];
        int y = a[row + 1][col];
        if (!used[min(x, y)][max(x, y)]) {
            used[min(x, y)][max(x, y)] = true;
            a[row][col] = -1;
            a[row + 1][col] = -1;
            result += rec(a, used, row, col + 1);
            used[min(x, y)][max(x, y)] = false;
            a[row][col] = x;
            a[row + 1][col] = y;
        }
    }
    return result;
}
void do_testcase() {
    int a[7][8];
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 8; j++) {
            cin >> a[i][j];
        }
    }
    bool used[7][7] = {false};
    cout << rec(a, used, 0, 0) << '\n';
}
int main() {
    int t; cin >> t;
    while (t--) do_testcase();
}
