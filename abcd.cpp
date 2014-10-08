// 2014-05-02
#include <cstdio>
char s[100001];
char t[100001];
char other(char a, char b, char c) {
    for (char d = 'A';; d++) {
        if (d != a && d != b && d != c) return d;
    }
}
int main() {
    int N; scanf("%d\n", &N);
    gets(s);
    for (int i = 0; i < N; i++) {
        t[2*i] = other(s[2*i], s[2*i+1], i == 0 ? 'A' : t[2*i-1]);
        t[2*i+1] = other(t[2*i], s[2*i], s[2*i+1]);
    }
    t[2*N] = 0;
    puts(t);
}
