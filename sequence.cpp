// 2024-09-02
#include <ctype.h>
#include <map>
#include <set>
#include <stdio.h>
#include <string>
#include <unordered_map>
using namespace std;
int main() {
    unordered_map<string, int> freq;
    string word;
    for (;;) {
        int c = getchar();
        if (c == EOF) break;
        if (islower(c)) {
            c = toupper(c);
        }
        if (isupper(c)) {
            word.push_back(c);
        } else {
            word.clear();
        }
        for (int i = 1; i <= 5; i++) {
            if (i <= word.size()) {
                freq[word.substr(word.size() - i)]++;
            }
        }
    }
    map<int, set<string>> ifreq[6];
    for (const auto& p : freq) {
        const int l = p.first.size();
        ifreq[l][p.second].insert(p.first);
    }
    for (int i = 1; i <= 5; i++) {
        printf("Analysis for Letter Sequences of Length %d\n", i);
        printf("-----------------------------------------\n");
        auto j = ifreq[i].end();
        for (int rank = 1; rank <= 5; ++rank) {
            if (j == ifreq[i].begin()) break;
            --j;
            printf("Frequency = %d, Sequence(s) = (", j->first);
            bool first = true;
            for (const auto& seq : j->second) {
                if (!first) putchar(',');
                first = false;
                printf("%s", seq.c_str());
            }
            puts(")");
        }
        if (i < 5) putchar('\n');
    }
}
