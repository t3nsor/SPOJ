// 2014-05-29
#include <cstdio>
#include <utility>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
int BIT[30001];
int n;
int loc[1000001];
int query(int i) {
    int res = 0;
    while (i > 0) {
        res += BIT[i];
        i -= i & -i;
    }
    return res;
}
int sum = 0;
void update(int i, int x) {
    sum += x;
    while (i <= n) {
        BIT[i] += x;
        i += i & -i;
    }
}
int a[30001];
int main() {
    memset(loc, -1, sizeof(loc));
    scanf("%d", &n);
    vector<pair<int, int> > events;
    for (int i = 1; i <= n; i++) {
        scanf("%d", a+i);
        events.push_back(make_pair(i, -1));
    }
    int q; scanf("%d", &q);
    vector<pair<int, int> > queries;
    vector<int> answers(q);
    for (int i = 0; i < q; i++) {
        int a, b; scanf("%d %d", &a, &b);
        queries.push_back(make_pair(a, b));
        events.push_back(make_pair(b, i));
    }
    sort(events.begin(), events.end());
    for (int i = 0; i < events.size(); i++) {
        pair<int, int> evt = events[i];
        if (evt.second == -1) {
            int j = evt.first;
            if (loc[a[j]] != -1) {
                update(loc[a[j]], -1);
            }
            update(j, 1);
            loc[a[j]] = j;
        } else {
            int l = queries[evt.second].first;
            answers[evt.second] = sum - query(l - 1);
        }
    }
    for (int i = 0; i < q; i++) {
        printf("%d\n", answers[i]);
    }
}
