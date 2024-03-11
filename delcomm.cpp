// 2024-03-10
// Pick one of the files that has to be deleted, and generate all possible
// patterns that match its basename (I am using this to mean the part of the
// filename before the period, I am not sure whether this is the correct
// terminology) and all possible patterns that match its extension. Then, for
// each ordered pair of such patterns, check whether it is a valid solution.
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;
int get() {
    int val = 0;
    char c;
    do {
        c = getchar();
    } while (c <= 32);
    do {
        val = 10*val + c - '0';
        c = getchar();
    } while (c > 32);
    return val;
}
struct filename {
    string base;
    string ext;
};
vector<string> generate_patterns(string s) {
    vector<string> result;
    for (int suffix = 0; suffix <= s.size(); suffix++) {
        const int prefix = s.size() - suffix;
        for (int i = 0; i < (1 << prefix); i++) {
            string pattern = s.substr(0, prefix);
            for (int j = 0; j < prefix; j++) {
                if (i & (1 << j)) {
                    pattern[j] = '?';
                }
            }
            result.push_back(pattern + '*');
            if (suffix == 0) result.push_back(pattern);
        }
    }
    return result;
}
bool is_match_nonstar(string pattern, string subject) {
    for (int i = 0; i < pattern.size(); i++) {
        if (pattern[i] != '?' && pattern[i] != subject[i]) return false;
    }
    return true;
}
bool is_match(string pattern, string subject) {
    if (pattern.empty()) return subject.empty();
    if (pattern.back() == '*') {
        pattern.pop_back();
        if (subject.size() < pattern.size()) return false;
        subject.resize(pattern.size());
    }
    if (pattern.size() != subject.size()) return false;
    return is_match_nonstar(pattern, subject);
}
void do_testcase() {
    char c;
    do {
        c = getchar();
    } while (c <= 32);
    vector<filename> del, keep;
    do {
        string name;
        const char disposition = c;
        c = getchar();
        while (c > 32) {
            name.push_back(c);
            c = getchar();
        }
        const int dotpos = name.find('.');
        filename f;
        if (dotpos == name.npos) {
            f.base = name;
        } else {
            f.base = name.substr(0, dotpos);
            f.ext = name.substr(dotpos + 1);
        }
        if (disposition == '-') {
            del.push_back(f);
        } else {
            keep.push_back(f);
        }
        c = getchar();
    } while (c > 32);
    const auto bcands = generate_patterns(del[0].base);
    const auto ecands = generate_patterns(del[0].ext);
    for (const auto& bcand : bcands) {
        for (const auto& ecand : ecands) {
            for (const auto& filename : del) {
                if (!is_match(bcand, filename.base) ||
                    !is_match(ecand, filename.ext)) goto fail;
            }
            for (const auto& filename : keep) {
                if (is_match(bcand, filename.base) &&
                    is_match(ecand, filename.ext)) goto fail;
            }
            printf("DEL %s.%s\n", bcand.c_str(), ecand.c_str());
            return;
fail:
            ;
        }
    }
    puts("IMPOSSIBLE");
}
int main() {
    int M = get();
    for (int i = 0; i < M; i++) {
        do_testcase();
        if (i < M - 1) putchar('\n');
    }
}
