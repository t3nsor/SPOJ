// 2024-04-03
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;
unsigned calc(vector<vector<unsigned>>& memo, int m, int s, int target) {
    unsigned& result = memo[s][target];
    if (result == -1) {
        result = 0;
        if (s == 1) {
            for (int i = 0; i <= 9; i++) {
                if (i % m == target) ++result;
            }
        } else if (s == 2) {
            for (int i = 0; i <= 99; i += 11) {
                if (i % m == target) ++result;
            }
        } else {
            int power = 1;
            for (int i = 0; i < s - 1; i++) {
                power = 10 * power % m;
            }
            power = (power + 1) % m;
            for (int firstdigit = 0; firstdigit <= 9; firstdigit++) {
                int newtarget = ((target - power * firstdigit) % m + m) % m;
                for (int i = 0; i < m; i++) {
                    if (i * 10 % m == newtarget) {
                        result += calc(memo, m, s - 2, i);
                    }
                }
            }
        }
    }
    return result;
}
void do_testcase() {
    int m, s; cin >> m >> s;
    if (s == 1) {
        int result = 0;
        for (int i = 1; i <= 9; i++) {
            if (i % m == 0) ++result;
        }
        cout << result << '\n';
        return;
    }
    if (s == 2) {
        int result = 0;
        for (int i = 11; i <= 99; i += 11) {
            if (i % m == 0) ++result;
        }
        cout << result << '\n';
        return;
    }
    unsigned result = 0;
    int power = 1;
    for (int i = 0; i < s - 1; i++) {
        power = 10 * power % m;
    }
    power = (power + 1) % m;
    vector<vector<unsigned>> memo(18, vector<unsigned>(999, -1));
    for (int firstdigit = 1; firstdigit <= 9; firstdigit++) {
        const int target = (-power * firstdigit % m + m) % m;
        for (int i = 0; i < m; i++) {
            if (i * 10 % m == target) {
                result += calc(memo, m, s - 2, i);
            }
        }
    }
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
