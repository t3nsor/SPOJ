// 2026-07-18
// nothing complicated, just hash tables (I assume a collision is unlikely)
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;
int N, L;
vector<string> words;
// list of lengths at which the suffix of the word with the first index
// matches the prefix of the word with the second index
vector<vector<vector<int>>> match;
vector<int> vis;
void dfs(int x) {
    vis[x] = 1;
    for (int i = 0; i < words.size(); i++) {
        if (!vis[i] && !match[x][i].empty()) dfs(i);
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin >> N >> L;
    match.resize(N);
    for (int i = 0; i < N; i++) {
        string s; cin >> s;
        words.push_back(s);
        match[i].resize(N);
    }
    for (int l = 1; l <= 128; l++) {
        unordered_map<string, vector<int>> prefix;
        for (int i = 0; i < N; i++) {
            if (words[i].size() < l) continue;
            prefix[words[i].substr(0, l)].push_back(i);
        }
        for (int i = 0; i < N; i++) {
            if (words[i].size() < l) continue;
            auto it = prefix.find(words[i].substr(words[i].size() - l));
            if (it == prefix.end()) continue;
            for (const auto j : it->second) {
                match[i][j].push_back(l);
            }
        }
    }
    vis.resize(N);
    dfs(0);
    unordered_set<string> S;
    for (int i = 0; i < N; i++) {
        if (!vis[i]) continue;
        const auto& w1 = words[i];
        for (int k = 0; k + L <= w1.size(); k++) S.insert(w1.substr(k, L));
        for (int j = 0; j < N; j++) {
            const auto& w2 = words[j];
            for (const auto l : match[i][j]) {
                if (l > L - 2) continue;
                // `k` is the number of unshared characters to take from the
                // first word, `l` is the number of shared characters, and we
                // must take at least 1 character from the second word
                for (int k = 1; k + l < L; k++) {
                    S.insert(w1.substr(w1.size() - k - l) +
                             w2.substr(l, L - k - l));
                }
            }
        }
    }
    cout << S.size() << '\n';
}
