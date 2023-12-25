// 2008-06-30
// updated 2023-12-25 for legibility
// The file jceasy_strings.txt is the list of strings given in the problem
// description, and jceasy_prog.txt consists of the strings from the `s` array
// in the program (the "input file"), one per line, without quotation marks.
// The output is the text file that should be submitted (i.e. a test case where
// the program gives the right output).
// I don't remember how this works :)
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int seq[100];
int N;
string s1[335];
string s2[335];
bool eq(const string& str, const string& progstr) {
    if (str.length() != progstr.length()) return false;
    if (str == "2") return true;
    for (int i = 0; i < str.length(); i += 2) {
        if (str[i] != progstr[i + 1]) return false;
    }
    return true;
}
bool dfs(int x) {
    int i;
    if (x==334) return true;
    if (N==100) return false;
    for (i=1; i<=334; i++) {
        if (eq(s1[i], s2[x])) {
            seq[N++]=i;
            if (dfs(i)) return true; else N--;
        }
    }
    return false;
}
int main() {
    int i;
    ifstream f1("jceasy_strings.txt");
    ifstream f2("jceasy_prog.txt");
    for (i=1; i<=334; i++) {
        f1 >> s1[i];
        f2 >> s2[i];
    }
    seq[0]=333;
    N=1;
    if (!dfs(333)) {
        cerr << "Solution not found\n";
        return 1;
    }
    cout << N << ' ';
    for (i=0; i<N; i++) {
        cout << seq[i] << ' ';
    }
    cout << '\n';
}
