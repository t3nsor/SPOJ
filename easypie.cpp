// 2022-08-06
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;
struct UserProblemInfo {
    int accepted_sec = -1;
    int num_tries = 0;
};
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) {
        int n; cin >> n;
        unordered_map<char, unordered_map<string, UserProblemInfo>> info_map;
        while (n--) {
            int sec;
            string team;
            string problem;
            string result;
            cin >> sec >> team >> problem >> result;
            auto& info = info_map[problem[0]][team];
            if (info.accepted_sec >= 0) continue;
            info.num_tries++;
            if (result[0] == 'A') {
                info.accepted_sec = sec;
            }
        }
        for (char c = 'A'; c <= 'I'; c++) {
            const auto& problem_info_map = info_map[c];
            int accepted_teams = 0;
            int tries_until_accepted = 0;
            int sec_until_accepted = 0;
            for (const auto& kv : problem_info_map) {
                const auto& info = kv.second;
                if (info.accepted_sec >= 0) {
                    ++accepted_teams;
                    tries_until_accepted += info.num_tries;
                    sec_until_accepted += info.accepted_sec;
                }
            }
            cout << c << ' ' << accepted_teams;
            if (accepted_teams) {
                cout << fixed << setprecision(2)
                     << ' ' << (tries_until_accepted)/double(accepted_teams)
                     << ' ' << (sec_until_accepted)/double(accepted_teams);
            }
            cout << '\n';
        }
    }
}
