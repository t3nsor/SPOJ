// 2026-04-24
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;
map<string, int> wordval = {
  {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}, {"six", 6},
  {"seven", 7}, {"eight", 8}, {"nine", 9}, {"ten", 10}, {"eleven", 11},
  {"twelve", 12}, {"thirteen", 13}, {"fourteen", 14}, {"fifteen", 15},
  {"sixteen", 16}, {"seventeen", 17}, {"eighteen", 18}, {"nineteen", 19},
  {"twenty", 20}, {"thirty", 30}, {"forty", 40}, {"fifty", 50}, {"sixty", 60},
  {"seventy", 70}, {"eighty", 80}, {"ninety", 90}
};
int getnum2(vector<string> words) {
    int result = 0;
    auto it = find(words.begin(), words.end(), "hundred");
    if (it != words.end()) {
        result += 100*wordval[it[-1]];
        words.erase(words.begin(), it + 1);
    }
    for (const auto& word : words) result += wordval[word];
    return result;
}
int getnum(string s) {
    int result = 0;
    vector<string> words;
    string word;
    for (const char c : s) {
        if (c == '-') {
            words.push_back(word);
            word.clear();
        } else {
            word.push_back(c);
        }
    }
    if (!word.empty()) words.push_back(word);
    if (words.size() >= 2 && words[1] == "million") {
        result += 1000000;
        words.erase(words.begin(), words.begin() + 2);
    }
    auto it = find(words.begin(), words.end(), "thousand");
    if (it != words.end()) {
        vector<string> thousands(words.begin(), it);
        result += 1000*getnum2(thousands);
        words.erase(words.begin(), it + 1);
    }
    return result + getnum2(words);
}
int calc(int X) {
    int node = 0;
    int from;
    while (X > 0) {
        //cerr << "DEBUG: X=" << X << "; node=" << node << '\n';
        if (node == 0) {
            from = 0;
            const int Y = (X-- % 1000) / 100;
            node = (Y <= 7 ? 7 : 9);
        } else if (node == 1) {
            return X;
        } else if (node == 2) {
            const int Y = X-- % 3;
            if (Y == 0) {
                node = 7;
            } else if (Y == 1) {
                node = 1;
            } else {
                node = 4;
            }
        } else if (node == 4) {
            string s = to_string(X);
            reverse(s.begin(), s.end());
            const int Y = stoi(s);
            node = (Y > X-- ? 6 : 2);
        } else if (node == 6) {
            int digits = 1;
            int Xcopy = X--;
            while (Xcopy >= 10) {
                ++digits;
                Xcopy /= 10;
            }
            node = (digits % 2 == 0 ? 4 : 7);
        } else if (node == 7) {
            const long long Xl = X--;
            const int Y = Xl*Xl % 7;
            if (Y == 0) {
                node = 2;
            } else if (Y == 1) {
                node = 6;
            } else if (Y == 2) {
                node = 8;
            } else {
                node = 0;
            }
        } else if (node == 8) {
            from = 8;
            const int Y = X-- % 5;
            node = (Y == 2 || Y == 3 ? 7 : 9);
        } else {
            X--;
            node = 8 - from;
        }
    }
    return -1;
}
int main() {
    string s;
    while (getline(cin, s)) {
        const int num = getnum(s);
        //cerr << "DEBUG: INPUT IS " << s << '\n';
        //cerr << "DEBUG: INPUT VALUE IS " << num << '\n';
        cout << calc(num) << '\n';
    }
}
