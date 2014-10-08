// 2014-04-27
// modified 2014-10-07
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <sstream>
#include <cstring>
#include <cctype>
using namespace std;
int main() {
    for (;;) {
        int n; cin >> n; if (n == 0) return 0;
        set<string> insignificant;
        for (int i = 0; i < n; i++) {
            string s; cin >> s; insignificant.insert(s);
        }
        for (;;) {
            string acro; cin >> acro; cin.ignore();
            string phrase; getline(cin, phrase);
            if (phrase == "CASE") break;
            istringstream iss(phrase);
            vector<string> words;
            for (;;) {
                string word; iss >> word;
                if (word == "") break;
                if (insignificant.find(word) == insignificant.end()) {
                    words.push_back(word);
                }
            }
            // dp[i][j] is the number of ways to use the first i words and the
            // first j letters of the acronym
            int dp[151][151]; memset(dp, 0, sizeof(dp));
            dp[0][0] = 1;
            int dp2[151][151];
            for (int i = 0; i < words.size(); i++) {
                for (int j = 0; j < acro.length(); j++) {
                    // How many letters of this word do we want to use?
                    int mx = min(acro.length() - j, words[i].length());
                    for (int k = 0; k <= words[i].length(); k++) {
                        dp2[k][0] = 1;
                    }
                    for (int k = 1; k <= mx; k++) {
                        dp2[0][k] = 0;
                    }
                    for (int k = 0; k < words[i].length(); k++) {
                        for (int m = 0; m < mx; m++) {
                            dp2[k+1][m+1] = dp2[k][m+1];
                            if (words[i][k] == tolower(acro[j+m])) {
                                dp2[k+1][m+1] += dp2[k][m];
                            }
                        }
                    }
                    for (int k = 1; k <= mx; k++) {
                        dp[i+1][j+k] += dp[i][j] * dp2[words[i].length()][k];
                    }
                }
            }
            if (dp[words.size()][acro.length()] == 0) {
                cout << acro << " is not a valid abbreviation" << endl;
            } else {
                cout << acro << " can be formed in "
                     << dp[words.size()][acro.length()] << " ways" << endl;
            }
        }
    }
}
