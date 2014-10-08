// 2014-04-25
#include <iostream>
#include <cstdio>
#include <string>
#include <map>
using namespace std;
int main() {
    int N;
    cin >> N;
    while (N--) {
        int stupid; scanf("%d\n", &stupid);
        map<string, int> M;
        string s; cin >> s;
        for (int i = 0; i < 38; i++) {
            M[s.substr(i, 3)]++;
        }
        printf("%d %d %d %d %d %d %d %d %d\n", stupid,
               M["TTT"], M["TTH"], M["THT"], M["THH"],
               M["HTT"], M["HTH"], M["HHT"], M["HHH"]);
    }
}
