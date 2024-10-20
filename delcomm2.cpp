// 2024-10-19
// For the strings "a", "ab", ..., "abcdefgh", precompute all patterns that
// match each such string. These are the "templates" for a given length. Now,
// given some filename we want to delete, all patterns that match that
// filename can be obtained by taking the templates for the filename's length
// and replacing each character "a" ... "h" with the corresponding character
// from the filename. In this way we can build up a set of all patterns that
// match at least 2 of the files marked for deletion. Then, we pick the one that
// matches the greatest number of files marked for deletion while not matching
// any files required to be kept. To determine whether a pattern matches a file
// required to be kept, we use DP. I don't know if this would be faster if we
// built a DFA using the powerset construction; I didn't bother trying it.
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
void generate_templates_helper(vector<string>& result,
                              const string& s,
                              int i,
                              string cur) {
    if (i == s.size()) {
        result.push_back(move(cur));
        return;
    }
    generate_templates_helper(result, s, i + 1, cur + s[i] + '*');
    generate_templates_helper(result, s, i + 1, cur + s[i]);
    if (cur.empty() || cur.back() != '*') {
        generate_templates_helper(result, s, i + 1, cur + '*');
        generate_templates_helper(result, s, i + 1, cur + "?*");
        generate_templates_helper(result, s, i + 1, cur + '?');
    } else {
        generate_templates_helper(result, s, i + 1, cur);
    }
}
vector<string> generate_templates(string s) {
    vector<string> result;
    generate_templates_helper(result, s, 0, "");
    generate_templates_helper(result, s, 0, "*");
    result.erase(remove_if(result.begin(),
                           result.end(),
                           [&] (const string& pattern) {
                               // As a small optimization we remove any
                               // templates that have 8 non-stars and also
                               // at least one star.
                               const int num_stars = count(pattern.begin(),
                                                           pattern.end(),
                                                           '*');
                               return num_stars > 0 &&
                                      pattern.size() - num_stars == 8;
                           }),
                 result.end());
    sort(result.begin(), result.end());
    result.erase(unique(result.begin(), result.end()), result.end());
    return result;
}
vector<string> templates[9] = {
    vector<string>(),
    generate_templates("a"),
    generate_templates("ab"),
    generate_templates("abc"),
    generate_templates("abcd"),
    generate_templates("abcde"),
    generate_templates("abcdef"),
    generate_templates("abcdefg"),
    generate_templates("abcdefgh")
};
bool is_match(string pattern, string subject) {
    bool dp[16][9];
    dp[0][0] = true;
    for (int i = 1; i <= subject.size(); i++) {
        dp[0][i] = false;
    }
    for (int i = 1; i <= pattern.size(); i++) {
        dp[i][0] = dp[i - 1][0] && pattern[i - 1] == '*';
    }
    for (int i = 1; i <= pattern.size(); i++) {
        for (int j = 1; j <= subject.size(); j++) {
            if (pattern[i - 1] == '?') {
                dp[i][j] = dp[i - 1][j - 1];
            } else if (pattern[i - 1] == '*') {
                dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
            } else {
                dp[i][j] = dp[i - 1][j - 1] &&
                           (pattern[i - 1] == subject[j - 1]);
            }
        }
    }
    return dp[pattern.size()][subject.size()];
}
void do_testcase() {
    string line;
    vector<string> del, keep;
    for (;;) {
        getline(cin, line);
        if (line.empty()) break;
        vector<string>& v = (line.back() == '+' ? del : keep);
        v.push_back(line.substr(0, line.size() - 2));
    }
    // generate all patterns that match at least one of the strings that need
    // to be deleted, and count how many of the files to delete they match
    unordered_map<string, int> patterns;
    for (const auto& s : del) {
        // generate patterns that match `s`
        vector<string> v;
        for (auto pattern : templates[s.size()]) {
            for (char& c : pattern) if (c >= 'a' && c <= 'h') c = s[c - 'a'];
            v.push_back(move(pattern));
        }
        sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end()), v.end());
        for (const auto& pattern : v) {
            patterns[pattern]++;
        }
    }
    vector<vector<string>> patterns_for_count(251);
    for (auto& p : patterns) {
        patterns_for_count[p.second].push_back(p.first);
    }
    // randomize the order of the files that need to be kept
    srand(12345);
    random_shuffle(keep.begin(), keep.end());
    // try patterns in reverse order of how many files to be deleted they match
    for (int i = 250; i >= 2; i--) {
        for (const auto& pattern : patterns_for_count[i]) {
            bool success = true;
            for (const auto& filename : keep) {
                if (is_match(pattern, filename)) {
                    success = false;
                    break;
                }
            }
            if (success) {
                cout << i << '\n';
                return;
            }
        }
    }
    // We know that the answer is always at least 1 because we can just use the
    // literal filename of one of the files marked for deletion.
    cout << "1\n";
}
int main() {
    string line; getline(cin, line);
    istringstream iss(line); int M; iss >> M;
    getline(cin, line);
    while (M--) do_testcase();
}
