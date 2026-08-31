// 2026-08-30
#include <algorithm>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;
string n1[] = {"zero", "one", "two", "three", "four",
               "five", "six", "seven", "eight", "nine",
               "ten", "eleven", "twelve", "thirteen", "fourteen",
               "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
string n2[] = {"", "", "twenty", "thirty", "forty",
               "fifty", "sixty", "seventy", "eighty", "ninety"};
int parse_num(const vector<string>& words) {
    int result = 0;
    for (const auto& word : words) {
        auto it = find(begin(n1), end(n1), word);
        if (it != end(n1)) {
            result += it - begin(n1);
            continue;
        }
        it = find(begin(n2), end(n2), word);
        if (it != end(n2)) {
            result += 10*(it - begin(n2));
            continue;
        }
    }
    return result;
}
vector<vector<string>> read_sentences() {
    vector<vector<string>> result;
    vector<string> sentence;
    string word;
    enum State {
        NORMAL,
        SINGLE_STRING,
        DOUBLE_STRING,
    };
    State state = NORMAL;
    for (;;) {
        const int c = getchar_unlocked();
        if (c == EOF) break;
        if (state == NORMAL) {
            if (c == '\'') {
                state = SINGLE_STRING;
            } else if (c == '"') {
                state = DOUBLE_STRING;
            } else if (c == ' ' || c == '\r' || c == '\n' || c == '.') {
                if (!word.empty()) {
                    sentence.emplace_back();
                    swap(sentence.back(), word);
                }
                if (c == '.') {
                    result.emplace_back();
                    swap(result.back(), sentence);
                }
            } else {
                word.push_back(c);
            }
        } else if (state == SINGLE_STRING) {
            if (c == '\'') {
                state = NORMAL;
                sentence.emplace_back();
                swap(sentence.back(), word);
            } else {
                word.push_back(c);
            }
        } else {  // DOUBLE_STRING
            if (c == '"') {
                state = NORMAL;
                sentence.emplace_back();
                swap(sentence.back(), word);
            } else {
                word.push_back(c);
            }
        }
    }
    return result;
}
void translate(const vector<string>& sentence) {
    int cur = 0;
    if (cur == sentence.size() || sentence[cur] != "The") throw;
    cur++;
    if (cur == sentence.size()) throw;
    if (sentence[cur] != "first" && sentence[cur] != "next") throw;
    cur++;
    if (cur == sentence.size()) throw;
    int nline = 0;
    if (sentence[cur] == "line") {
        nline++;
        cur++;
    } else {
        vector<string> numstr;
        while (cur < sentence.size() && sentence[cur] != "lines") {
            numstr.push_back(sentence[cur++]);
        }
        if (cur == sentence.size()) throw;
        cur++;
        nline = parse_num(numstr);
    }
    if (cur == sentence.size()) throw;
    if (sentence[cur] == "is" || sentence[cur] == "are") {
        cur++;
        if (cur == sentence.size() || sentence[cur] != "empty") throw;
        cur++;
        if (cur != sentence.size()) throw;
        while (nline--) putchar('\n');
        return;
    }
    if (sentence[cur] != "contain" && sentence[cur] != "contains") throw;
    cur++;
    string oneline;
    bool expect_followedby = false;
    enum Kind {
        NUMBER,
        STRING,
        SPACE,
        NONE
    } lastkind = NONE;
    while (cur != sentence.size()) {
        if (sentence[cur] == "followed") {
            if (!expect_followedby) throw;
            cur++;
            if (cur == sentence.size() || sentence[cur] != "by") throw;
            cur++;
            expect_followedby = false;
            continue;
        }
        else if (expect_followedby) throw;
        expect_followedby = true;
        int rep = 1;
        vector<string> repstr;
        while (cur != sentence.size() &&
               sentence[cur] != "number" && sentence[cur] != "numbers" &&
               sentence[cur] != "string" && sentence[cur] != "strings" &&
               sentence[cur] != "space" && sentence[cur] != "spaces") {
            repstr.push_back(sentence[cur++]);
        }
        if (cur == sentence.size()) throw;
        if (!repstr.empty()) rep = parse_num(repstr);
        if (sentence[cur] == "space" || sentence[cur] == "spaces") {
            while (rep--) oneline.push_back(' ');
            cur++;
            lastkind = SPACE;
            continue;
        }
        if (sentence[cur] == "string" || sentence[cur] == "strings") {
            cur++;
            if (cur == sentence.size()) throw;
            if (lastkind == NUMBER) oneline.push_back(' ');
            while (rep--) oneline.append(sentence[cur]);
            cur++;
            lastkind = STRING;
            continue;
        }
        // numbers go until the end of the sentence or "followed by"
        vector<string> numstr;
        cur++;
        while (cur != sentence.size() && sentence[cur] != "followed") {
            numstr.push_back(sentence[cur++]);
        }
        if (numstr.empty()) throw;
        const string s = to_string(parse_num(numstr));
        if (lastkind == NUMBER || lastkind == STRING) oneline.push_back(' ');
        lastkind = NUMBER;
        for (int i = 0; i < rep; i++) {
            if (i > 0) oneline.push_back(' ');
            oneline.append(s);
        }
    }
    if (!expect_followedby) throw;
    while (nline--) puts(oneline.c_str());
}
int main() {
    // The first step is to convert the input into sentences.  A sentence is a
    // sequence of tokens.  A token is either a word or a string.
    vector<vector<string>> sentences = read_sentences();
    // After this we just process sentences one by one.
    for (const auto& sentence : sentences) translate(sentence);
}
