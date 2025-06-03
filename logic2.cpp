// 2025-06-03
// Ignore the comments section of the problem page. Whatever illogical test data
// might have been present at one point seems to have been fixed.
//
// This solution assumes that a true statement that X is not guilty logically
// implies that the organizer is one of the M individuals other than X. I
// haven't tested whether you can get AC without this, but I did check that you
// can get AC without contriving some way to get mud's first test case to work.
// Similarly, you can ignore the idea from the comments section that says that
// the answer is "Impossible" only if a single person's statements, taken in
// isolation, can be neither all true nor all false.
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
enum Kind {
    GUILTY,
    NOT_GUILTY,
    TODAY,
};
struct Statement {
    int speaker;
    Kind kind;
    int subject;
};
void do_testcase(int M, int N, int P) {
    vector<string> names(M);
    unordered_map<string, int> id;
    for (int i = 0; i < M; i++) {
        cin >> names[i];
        id[names[i]] = i;
    }
    vector<Statement> stmts;
    for (int i = 0; i < P; i++) {
        string speaker; cin >> speaker; speaker.pop_back();
        Statement stmt{};
        stmt.speaker = id[speaker];
        cin.ignore();  // eat whitespace
        string line; getline(cin, line);
        if (line == "I am guilty.") {
            stmt.subject = stmt.speaker;
            stmt.kind = GUILTY;
            stmts.push_back(stmt);
            continue;
        }
        if (line == "I am not guilty.") {
            stmt.subject = stmt.speaker;
            stmt.kind = NOT_GUILTY;
            stmts.push_back(stmt);
            continue;
        }
        istringstream iss(line);
        string word; iss >> word;
        if (word == "Today") {
            iss >> word;
            if (word != "is") continue;
            iss >> word;
            if (word == "Monday.") {
                stmt.subject = 0;
            } else if (word == "Tuesday.") {
                stmt.subject = 1;
            } else if (word == "Wednesday.") {
                stmt.subject = 2;
            } else if (word == "Thursday.") {
                stmt.subject = 3;
            } else if (word == "Friday.") {
                stmt.subject = 4;
            } else if (word == "Saturday.") {
                stmt.subject = 5;
            } else if (word == "Sunday.") {
                stmt.subject = 6;
            } else continue;
            iss >> word;
            if (iss.eof()) {
                stmt.kind = TODAY;
                stmts.push_back(stmt);
            }
            continue;
        }
        const auto it = id.find(word);
        if (it == id.end()) continue;
        stmt.subject = it->second;
        iss >> word;
        if (word != "is") continue;
        iss >> word;
        if (word == "guilty.") {
            iss >> word;
            if (iss.eof()) {
                stmt.kind = GUILTY;
                stmts.push_back(stmt);
            }
            continue;
        }
        if (word != "not") continue;
        iss >> word;
        if (word == "guilty.") {
            iss >> word;
            if (iss.eof()) {
                stmt.kind = NOT_GUILTY;
                stmts.push_back(stmt);
            }
        }
    }
    int orgmask = 0;
    for (int liarmask = 0; liarmask < (1 << M); liarmask++) {
        if (__builtin_popcount(liarmask) != N) continue;
        int daymask = 127;
        for (const auto& stmt : stmts) {
            if (stmt.kind != TODAY) continue;
            const bool lie = liarmask & (1 << stmt.speaker);
            if (lie) {
                daymask &= 127 ^ (1 << stmt.subject);
            } else {
                daymask &= 1 << stmt.subject;
            }
        }
        if (daymask == 0) continue;
        int curorgmask = (1 << M) - 1;
        for (const auto& stmt : stmts) {
            if (stmt.kind == TODAY) continue;
            const bool is_guilty = (stmt.kind == GUILTY) ^
                                   bool(liarmask & (1 << stmt.speaker));
            if (is_guilty) {
                curorgmask &= 1 << stmt.subject;
            } else {
                curorgmask &= ((1 << M) - 1) ^ (1 << stmt.subject);
            }
        }
        orgmask |= curorgmask;
    }
    if (__builtin_popcount(orgmask) == 0) {
        cout << "Impossible\n";
    } else if (__builtin_popcount(orgmask) == 1) {
        cout << names[__builtin_ctz(orgmask)] << '\n';
    } else {
        cout << "Cannot Determine\n";
    }
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        int M, N, P; cin >> M >> N >> P;
        if (cin.eof()) break;
        do_testcase(M, N, P);
    }
}
