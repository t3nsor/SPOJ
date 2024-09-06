// 2024-09-05
#include <ctype.h>
#include <initializer_list>
#include <stack>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;
struct quilt {
    int R;
    int C;
    vector<char> data;
    char* operator[](int r) {
        return data.data() + r * C;
    }
    const char* operator[](int r) const {
        return data.data() + r * C;
    }
};
char turn_char[128];
quilt turn(const quilt& q) {
    quilt result;
    result.R = q.C;
    result.C = q.R;
    result.data.resize(result.R * result.C);
    for (int i = 0; i < result.R; i++) {
        for (int j = 0; j < result.C; j++) {
            result[i][j] = turn_char[q[q.R - 1 - j][i]];
        }
    }
    return result;
}
quilt sew(const quilt& q1, const quilt& q2) {
    quilt result;
    if (q1.R != q2.R) throw -1;
    result.R = q1.R;
    result.C = q1.C + q2.C;
    result.data.resize(result.R * result.C);
    for (int i = 0; i < result.R; i++) {
        for (int j = 0; j < q1.C; j++) {
            result[i][j] = q1[i][j];
        }
        for (int j = 0; j < q2.C; j++) {
            result[i][j + q1.C] = q2[i][j];
        }
    }
    return result;
}
const quilt A = {2, 2, {'/', '/', '/', '+'}};
const quilt B = {2, 2, {'-', '-', '-', '-'}};
char get_token() {
    int c;
    do {
        c = getchar();
        if (c == EOF) return c;
    } while (isspace(c));
    if (c == 'A' || c == 'B' || c == ';' ||
        c == '(' || c == ')' || c == ',') {
        return c;
    }
    if (c == 's') {
        getchar(); getchar();
        return c;
    }
    if (c == 't') {
        getchar(); getchar(); getchar();
        return c;
    }
}
struct entry {
    char c;
    quilt q;
};
void do_testcase(const string& input) {
    stack<entry> S;
    try {
        for (const char c : input) {
            if (c == ',' || c == '(') continue;
            if (c == 's' || c == 't') {
                S.push(entry{c});
                continue;
            }
            if (c == 'A') {
                S.push(entry{0, A});
                continue;
            }
            if (c == 'B') {
                S.push(entry{0, B});
                continue;
            }
            const auto top = S.top(); S.pop();
            const auto top2 = S.top(); S.pop();
            if (top2.c == 't') {
                S.push(entry{0, turn(top.q)});
            } else {
                S.pop();
                S.push(entry{0, sew(top2.q, top.q)});
            }
        }
    } catch (int) {
        puts("error");
        return;
    }
    const auto& top = S.top();
    for (int i = 0; i < top.q.R; i++) {
        for (int j = 0; j < top.q.C; j++) {
            putchar(top.q[i][j]);
        }
        putchar('\n');
    }
}
int main() {
    turn_char['/'] = '\\';
    turn_char['\\'] = '/';
    turn_char['+'] = '+';
    turn_char['-'] = '|';
    turn_char['|'] = '-';
    int cs = 1;
    for (;;) {
        string input;
        for (;;) {
            const char c = get_token();
            if (c == ';') break;
            if (c == EOF) return 0;
            input.push_back(c);
        }
        printf("Quilt %d:\n", cs++);
        do_testcase(input);
    }
}
