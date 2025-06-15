// 2025-06-15
#include <algorithm>
#include <functional>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;
enum {
    NOT = 26,
    AND,
    OR,
    IMP,
    EQ,
};
struct Expr {
    int val;  // 0-25 are variables
    int pos;
    int left = -1;
    int right = -1;
};
int rec(int node, const vector<Expr>& exprs, string& line, int mask,
        const vector<int>& aidx) {
    int result;
    if (exprs[node].val <= 25) {
        result = bool(mask & (1 << aidx[exprs[node].val]));
    } else if (exprs[node].val == NOT) {
        result = !rec(exprs[node].left, exprs, line, mask, aidx);
    } else if (exprs[node].val == AND) {
        result = rec(exprs[node].left, exprs, line, mask, aidx) &
                 rec(exprs[node].right, exprs, line, mask, aidx);
    } else if (exprs[node].val == OR) {
        result = rec(exprs[node].left, exprs, line, mask, aidx) |
                 rec(exprs[node].right, exprs, line, mask, aidx);
    } else if (exprs[node].val == IMP) {
        result = !rec(exprs[node].left, exprs, line, mask, aidx) |
                 rec(exprs[node].right, exprs, line, mask, aidx);
    } else {
        result = rec(exprs[node].left, exprs, line, mask, aidx) ==
                 rec(exprs[node].right, exprs, line, mask, aidx);
    }
    line[exprs[node].pos] = result + '0';
    return result;
}
void do_testcase(const string& s) {
    vector<Expr> exprs;
    stack<int> operands;
    stack<pair<int, int>> operators;
    vector<int> atoms;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] <= 32 || s[i] == '(') {
            continue;
        } else if (s[i] >= 'a' && s[i] <= 'z') {
            Expr e{s[i] - 'a', i};
            exprs.push_back(e);
            operands.push(exprs.size() - 1);
            atoms.push_back(s[i] - 'a');
        } else if (s[i] == '!' || s[i] == '&' || s[i] == '|') {
            if (s[i] == '!') operators.emplace(NOT, i);
            else if (s[i] == '&') operators.emplace(AND, i);
            else operators.emplace(OR, i);
        } else if (s[i] == '<') {
            do { i++; } while (s[i] != '-');
            operators.emplace(EQ, i);
            do { i++; } while (s[i] != '>');
        } else if (s[i] == '-') {
            do { i++; } while (s[i] != '-');
            operators.emplace(IMP, i);
            do { i++; } while (s[i] != '>');
        } else {  // s[i] == ')'
            const auto o = operators.top();
            operators.pop();
            Expr expr = {o.first, o.second};
            if (o.first == NOT) {
                expr.left = operands.top();
                operands.pop();
            } else {
                expr.right = operands.top();
                operands.pop();
                expr.left = operands.top();
                operands.pop();
            }
            exprs.push_back(expr);
            operands.push(exprs.size() - 1);
        }
    }
    sort(atoms.begin(), atoms.end(), greater<void>());
    atoms.erase(unique(atoms.begin(), atoms.end()), atoms.end());
    vector<int> aidx(26, -1);
    for (int i = 0; i < atoms.size(); i++) {
        aidx[atoms[i]] = i;
    }
    const int root = operands.top();
    cout << s << '\n';
    for (int i = 0; i < (1 << atoms.size()); i++) {
        string line(s.size(), ' ');
        rec(root, exprs, line, i, aidx);
        cout << line << '\n';
    }
    cout << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        string s; getline(cin, s);
        if (cin.eof() ||
            find_if(s.begin(), s.end(), [](char c) { return c > 32; })
            == s.end()) break;
        do_testcase(s);
    }
}
