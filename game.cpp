// 2024-02-17
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

void do_match(int round,
              unordered_map<string, int>& teams,
              vector<pair<string, string>>& games) {
    cout << "Round #" << round << '\n';
    const int n = teams.size();
    if (n == 2) {
        cout << games[0].first << " defeats " << games[0].second << '\n';
        cout << "Winner: " << games[0].first << "\n\n";
        return;
    }
    // find teams that only play one game
    unordered_set<string> may_eliminate;
    for (const auto& kv : teams) {
        if (kv.second == 1) {
            may_eliminate.insert(kv.first);
        }
    }
    // Find opponents of teams that only play one game. If there are two teams
    // that each only appear once but have the same opponent, one of those two
    // teams has to be the wildcard (since the common opponent can't play two
    // games in the same round).
    string wildcard;
    int eliminate = n / 2;
    unordered_set<string> may_eliminate_opponents;
    for (const auto& game : games) {
        if (may_eliminate.count(game.first)) {
            if (may_eliminate_opponents.count(game.second)) {
                wildcard = game.first;
            } else {
                if (eliminate == 0) {
                    wildcard = game.first;
                } else {
                    --eliminate;
                    may_eliminate_opponents.insert(game.second);
                }
            }
        } else if (may_eliminate.count(game.second)) {
            if (may_eliminate_opponents.count(game.first)) {
                wildcard = game.second;
            } else {
                if (eliminate == 0) {
                    wildcard = game.second;
                } else {
                    --eliminate;
                    may_eliminate_opponents.insert(game.first);
                }
            }
        } 
    }
    for (auto& game : games) {
        if (may_eliminate.count(game.first) && game.first != wildcard) {
            cout << game.second << " defeats " << game.first << '\n';
            teams.erase(game.first);
            teams[game.second]--;
            game.first = game.second = "";
        } else if (may_eliminate.count(game.second) &&
                   game.second != wildcard) {
            cout << game.first << " defeats " << game.second << '\n';
            teams[game.first]--;
            teams.erase(game.second);
            game.first = game.second = "";
        }
    }
    // remove the games that were used in this round
    games.erase(remove(games.begin(), games.end(), pair<string, string>()),
                games.end());
    // It's possible that we still don't know who the wildcard is, because they
    // occurred more than once. In that case, the wildcard must be the only team
    // that didn't play any game against a team that was eliminated this round.
    if (n % 2) {
        if (wildcard.empty()) {
            for (const auto& kv : teams) {
                if (!may_eliminate_opponents.count(kv.first)) {
                    wildcard = kv.first;
                    break;
                }
            }
        }
        cout << wildcard << " advances with wildcard\n";
    }
    do_match(round + 1, teams, games);
}

int main() {
    ios::sync_with_stdio(false);
    int n;
    for (;;) {
        cin >> n; if (n == 0) return 0;
        unordered_map<string, int> teams;
        for (int i = 0; i < n; i++) {
            string s; cin >> s;
            teams[s] = 0;
        }
        vector<pair<string, string>> games;
        for (int i = 0; i < n - 1; i++) {
            string s1, s2; cin >> s1 >> s2;
            games.emplace_back(s1, s2);
            teams[s1]++;
            teams[s2]++;
        }
        do_match(1, teams, games);
    }
}
