// 2023-10-15
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
string o(string s) {
    string r;
    for(int i=0;i<s.size();i++) {
        r.push_back(s[i]-64);
    }
    return r;
}
string R[10][10] = {
{"", "m", "mm", "mmm", o("iv")},
{"", "c", "cc", "ccc", "cd", "d", "dc", "dcc", "dccc", "cm"},
{"", "x", "xx", "xxx", "xl", "l", "lx", "lxx", "lxxx", "xc"},
{"", o("M"), o("MM"), o("MMM"), "iv", "v", "vi", "vii", "viii", "ix"},
{"", o("C"), o("CC"), o("CCC"), o("CD"), o("D"), o("DC"), o("DCC"), o("DCCC"), o("CM")},
{"", o("X"), o("XX"), o("XXX"), o("XL"), o("L"), o("LX"), o("LXX"), o("LXXX"), o("XC")},
{"", "M", "MM", "MMM", o("IV"), o("V"), o("VI"), o("VII"), o("VIII"), o("IX")},
{"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"},
{"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"},
{"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"}
};
unsigned in() {
    string s,w;
    getline(cin,s);
    unsigned x=0;
    istringstream ss(s);
    while (ss>>w) {
        x*=10;
        switch (w[0]) {
            case 'O': x+=(w[1]=='N'); break;
            case 'E': x+=8; break;
            case 'N': x+=9; break;
            case 'T': x+=2+(w[1]=='H'); break;
            case 'F': x+=4+(w[1]=='I'); break;
            case 'S': x+=6+(w[1]=='E');
        }
    }
    return x;
}
int main() {
    int n=in();
    while (n--) {
        unsigned x=in();
        string r;
        unsigned p=1000000000;
        auto*P=&R[0];
        while (p>0) {
            r+=(*P)[x/p];
            x%=p;
            p/=10;
            ++P;
        }
        for (char c:r) {
            putchar(c<64?'_':' ');
        }
        puts("");
        for (char c:r) {
            putchar(c<64?c+64:c);
        }
        puts("");
    }
}
