// 2021-08-18
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
using namespace std;
struct predicate {
    string infinitive;
    vector<string> object;
    string conjugate(string subject, bool negative) {
        string result;
        if (negative) {
            if (subject == "I" || subject == "you") {
                result = "don't ";
            } else {
                result = "doesn't ";
            }
            result += infinitive;
        } else {
            result = infinitive;
            if (subject == "I" || subject == "you") {
                result.push_back('s');
            }
        }
        for (const auto& word : object) {
            result.push_back(' ');
            result += word;
        }
        return result;
    }
    bool operator==(const predicate& other) const {
        return infinitive == other.infinitive && object == other.object;
    }
};
struct information {
    string s;  // will never be "nobody"
    bool neg = false;
    predicate p;
};
string invert(string s) {
    if (s == "I") return "you";
    if (s == "you") return "I";
    return s;
}
void do_testcase() {
    bool contradiction = false;
    vector<information> data;
    for (;;) {
        string line; getline(cin, line);
        if (line.back() == '!') {
            cout << line << "\n\n";
            return;
        }
        vector<string> words;
        istringstream iss(line);
        // The problem description is not very clear about the input format.
        // But if you look at the actual test data, punctuation only occurs at
        // the end of lines.
        for (;;) {
            string word; iss >> word;
            words.push_back(word);
            if (ispunct(word.back())) break;
        }
        if (words.back().back() == '.') {
            // statement
            words.back().pop_back();
            information inf;
            inf.s = words[0];
            // The problem description says that "(nobody|everybody)
            // doesn't ..." will never occur. We also assume that "degenerate"
            // statements such as "I don't" and "everybody doesn't"
            // are illegal.
            int verb_index = 1;
            if (words[1] == "don't" || words[1] == "doesn't") {
                ++verb_index;
                inf.neg = true;
            }
            inf.p.infinitive = words[verb_index];
            if (inf.s != "I" && inf.s != "you" && !inf.neg) {
                inf.p.infinitive.pop_back();
            }
            for (int i = verb_index + 1; i < words.size(); i++) {
                inf.p.object.push_back(words[i]);
            }
            if (inf.s == "nobody") {
                inf.s = "everybody";
                // inf.neg can't be already true, since that would require
                // "nobody doesn't ..." which is illegal
                inf.neg = true;
            }
            for (const auto& inf2 : data) {
                if (!(inf2.p == inf.p)) continue;
                if ((inf.p == inf2.p) && (inf2.neg ^ inf.neg) &&
                    (inf.s == inf2.s ||
                     inf.s == "everybody" ||
                     inf2.s == "everybody")) {
                    contradiction = true;
                    break;
                }
            }
            data.push_back(move(inf));
        } else {
            // question; start by echoing it
            cout << line << '\n';
            if (contradiction) {
                cout << "I am abroad.\n\n";
                continue;
            }
            words.back().pop_back();
            if (words[0] == "do" || words[0] == "does") {
                // type 1
                const auto s = words[1];
                predicate p;
                p.infinitive = words[2];
                for (int i = 3; i < words.size(); i++) {
                    p.object.push_back(words[i]);
                }
                bool found_answer = false;
                bool neg;
                for (const auto& inf : data) {
                    if (!(inf.p == p)) continue;
                    if (inf.s == s || inf.s == "everybody") {
                        found_answer = true;
                        neg = inf.neg;
                    }
                }
                if (found_answer) {
                    if (neg) {
                        cout << "no, " << invert(s) << " ";
                        cout << (s == "I" || s == "you" ? "don't" : "doesn't");
                        cout << ' ' << p.infinitive;
                    } else {
                        cout << "yes, " << invert(s) << ' ' << p.infinitive;
                        if (s != "I" && s != "you") cout << 's';
                    }
                    for (const auto& ow : p.object) {
                        cout << ' ' << ow;
                    }
                } else {
                    cout << "maybe";
                }
            } else if (words[0] == "who") {
                // type 2
                predicate p;
                p.infinitive = words[1];
                // remove trailing s
                p.infinitive.pop_back();
                for (int i = 2; i < words.size(); i++) {
                    p.object.push_back(words[i]);
                }
                bool done = false;
                vector<string> pos;
                for (const auto& inf : data) {
                    if (inf.p == p) {
                        if (inf.s == "everybody") {
                            done = true;
                            if (inf.neg) {
                                cout << "nobody ";
                            } else {
                                cout << "everybody ";
                            }
                            cout << p.infinitive << "s";
                            for (const auto& ow : p.object) {
                                cout << ' ' << ow;
                            }
                            break;
                        } else if (!inf.neg) {
                            pos.push_back(inf.s);
                        }
                    }
                }
                if (!done) {
                    if (pos.empty()) {
                        cout << "I don't know";
                    } else {
                        for (int i = 0; i < pos.size(); i++) {
                            if (i > 0) {
                                if (i == pos.size() - 1) {
                                    cout << " and ";
                                } else {
                                    cout << ", ";
                                }
                            }
                            cout << invert(pos[i]);
                        }
                        cout << ' ' << p.infinitive;
                        if (pos.size() == 1 &&
                            pos[0] != "I" && pos[0] != "you") {
                            cout << 's';
                        }
                        for (const auto& ow : p.object) {
                            cout << ' ' << ow;
                        }
                    }
                }
            } else {
                // type 3
                // note that "what does/doesn't <subject> do" is illegal
                const auto s = words[2];
                vector<pair<predicate, bool>> result;
                for (const auto& inf : data) {
                    if (inf.s == s || inf.s == "everybody") {
                        result.emplace_back(inf.p, inf.neg);
                    }
                }
                if (result.empty()) {
                    cout << "I don't know";
                } else {
                    cout << invert(s);
                    for (int i = 0; i < result.size(); i++) {
                        if (i == 0) {
                            cout << ' ';
                        } else if (i == result.size() - 1 ) {
                            cout << ", and ";
                        } else {
                            cout << ", ";
                        }
                        if (result[i].second) {
                            cout << (s == "I" || s == "you"
                                     ? "don't" : "doesn't") << ' ';
                            cout << result[i].first.infinitive;
                        } else {
                            cout << result[i].first.infinitive;
                            if (s != "I" && s != "you") cout << 's';
                        }
                        for (const auto& ow : result[i].first.object) {
                            cout << ' ' << ow;
                        }
                    }
                }
            }
            cout << ".\n\n";
        }
    }
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T; cin.ignore();
    for (int i = 1; i <= T; i++) {
        cout << "Dialogue #" << i << ":\n";
        do_testcase();
    }
}
