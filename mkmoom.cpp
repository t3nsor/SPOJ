// 2025-11-02
// This problem has very few accepted users, probably because the problem
// description isn't very clear.  Some helpful clarifications are in the
// comments:
// 1. Angle brackets nest.
// 2. After replacing a macro by its definition (with each $\d replaced with the
//    corresponding argument) we have to rescan the resulting string for
//    further macros.
// The problem statement is also not clear about whether there is any difference
// between "scanning" (what we do to the input string and after each macro
// evaluation) and "evaluating" of arguments to macros.  To get AC, we have to
// assume that they are the same process, and each iteration of
// scanning/evaluating strips the outermost angle brackets.
//
// The problem statement is also a bit misleading when it gives the example
// where the result of macro expansion would be ",-[-$".  In reality this would
// be ill-formed for rescanning and therefore wouldn't occur in actual test
// data since we actually need to rescan.
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
unordered_map<string, string> macros;
vector<string> get_args(const string& s, int& i) {
    vector<string> result;
    int angledepth = 0;
    int macrodepth = 1;
    ++i;
    string piece;
    for (; macrodepth > 0 || angledepth > 0; i++) {
        if (s[i] == '<') {
            ++angledepth;
            piece += s[i];
            continue;
        }
        if (s[i] == '>') {
            --angledepth;
            piece += s[i];
            continue;
        }
        if (angledepth > 0) {
            piece += s[i];
            continue;
        }
        if (s[i] == '[') {
            ++macrodepth;
            piece += s[i];
            continue;
        }
        if (s[i] == ']') {
            --macrodepth;
            if (macrodepth > 0) piece += s[i]; else result.push_back(piece);
            continue;
        }
        if (s[i] == ',' && macrodepth == 1) {
            result.push_back(piece);
            piece.clear();
            continue;
        }
        piece += s[i];
    }
    return result;
}
string scan(const string& s);
string do_macro(vector<string> args) {
    for (auto& arg : args) {
        arg = scan(arg);
    }
    string result;
    if (args[0] == "def") {
        macros[move(args[1])] = move(args[2]);
        return result;
    }
    const string& defn = macros[args[0]];
    int angledepth = 0;
    for (int i = 0; i < defn.size(); i++) {
        if (defn[i] == '<') {
            ++angledepth;
            result += defn[i];
            continue;
        }
        if (defn[i] == '>') {
            --angledepth;
            result += defn[i];
            continue;
        }
        if (angledepth == 0 && defn[i] == '$') {
            i++;
            result += args[defn[i] - '0'];
            continue;
        }
        result += defn[i];
    }
    return scan(result);
}
string scan(const string& s) {
    int i = 0;
    int angledepth = 0;
    string result;
    while (i < s.size()) {
        if (angledepth > 0) {
            if (s[i] == '<') ++angledepth;
            else if (s[i] == '>') --angledepth;
            if (angledepth > 0) result += s[i];
            ++i;
            continue;
        }
        if (s[i] == '<') {
            ++angledepth;
            if (angledepth > 1) result += s[i];
            ++i;
            continue;
        }
        if (s[i] != '[') {
            result += s[i++];
            continue;
        }
        vector<string> args = get_args(s, i);
        result += do_macro(args);
    }
    return result;
}
void do_testcase(int cs, const string& input) {
    macros.clear();
    string output = scan(input);
    cout << "Case " << cs << '\n'
         << string(79, '-') << '\n'
         << output
         << string(79, '-') << '\n'
         << '\n';
}
int main() {
    for (int cs = 1;; cs++) {
        string line; getline(cin, line);
        int L = atoi(line.c_str());
        if (L == 0) break;
        string input;
        while (L--) {
            getline(cin, line);
            line.push_back('\n');
            input += line;
        }
        do_testcase(cs, input);
    }
}
