// 2024-03-11
#include <algorithm>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>
using namespace std;
struct Group {
    vector<int> members;
    int total_freq;
    int earliest;
};
void do_testcase(int R, int N) {
    const int orig_N = N;
    vector<int> freq(N);
    for (int i = 0; i < N; i++) {
        scanf("%d", &freq[i]);
    }
    while ((N - 1) % (R - 1) != 0) {
        N++;
        freq.push_back(0);
    }
    vector<Group> groups;
    for (int i = 0; i < N; i++) {
        groups.push_back({{i}, freq[i], i});
    }
    vector<string> encoding(N);
    while (groups.size() > 1) {
        sort(groups.begin(), groups.end(),
             [](const Group& group1, const Group& group2) {
                 return group1.total_freq < group2.total_freq ||
                        (group1.total_freq == group2.total_freq &&
                         group1.earliest < group2.earliest);
             });
        Group new_group;
        new_group.earliest = 100;
        new_group.total_freq = 0;
        for (int i = 0; i < R; i++) {
            new_group.members.insert(new_group.members.end(),
                                     groups[i].members.begin(),
                                     groups[i].members.end());
            new_group.total_freq += groups[i].total_freq;
            new_group.earliest = min(new_group.earliest, groups[i].earliest);
            for (const int mem : groups[i].members) {
                encoding[mem].push_back(i + '0');
            }
        }
        groups.erase(groups.begin(), groups.begin() + R);
        groups.push_back(move(new_group));
    }
    int total_len = 0;
    for (int i = 0; i < orig_N; i++) {
        total_len += freq[i] * encoding[i].length();
    }
    printf("average length %.2f\n", total_len / (double)groups[0].total_freq);
    for (int i = 0; i < orig_N; i++) {
        reverse(encoding[i].begin(), encoding[i].end());
        printf("    %c: %s\n", (char)('A' + i), encoding[i].c_str());
    }
}
int main() {
    int R;
    for (int i = 1; ; i++) {
        scanf("%d", &R);
        if (R == 0) return 0;
        int N;
        scanf("%d", &N);
        printf("Set %d; ", i);
        do_testcase(R, N);
        putchar('\n');
    }
}
