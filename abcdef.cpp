// 2014-04-25
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
int a[100];
int main() {
    int N;
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d", a+i);
    }
    vector<int> s1, s2;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                s1.push_back(a[i]*a[j]+a[k]);
                if (a[i] != 0) {
                    s2.push_back(a[i]*(a[j]+a[k]));
                }
            }
        }
    }
    sort(s1.begin(), s1.end());
    sort(s2.begin(), s2.end());
    long long res = 0; int i = 0, j = 0;
    while (i < s1.size() && j < s2.size()) {
        if (s1[i] < s2[j]) {
            i++;
        } else if (s1[i] > s2[j]) {
            j++;
        } else {
            int k, m;
            for (k = i; k < s1.size() && s1[k] == s1[i]; k++);
            for (m = j; m < s2.size() && s2[m] == s2[j]; m++);
            res += (k-i)*(long long)(m-j); i = k; j = m;
        }
    }
    printf("%lld\n", res);
}
