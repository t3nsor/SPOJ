// 2014-04-25
#include <vector>
#include <algorithm>
#include <cstdio>
using namespace std;
int a[50000];
int sum[200000];
int maxlsum[200000];
int maxrsum[200000];
int maxisum[200000];
int N, M;
void update(int node) {
    sum[node] = sum[2*node] + sum[2*node+1];
    maxlsum[node] = max(maxlsum[2*node], sum[2*node] + maxlsum[2*node+1]);
    maxrsum[node] = max(maxrsum[2*node+1], sum[2*node+1] + maxrsum[2*node]);
    maxisum[node] = max(max(maxisum[2*node], maxisum[2*node+1]),
                        maxrsum[2*node] + maxlsum[2*node+1]);
}
void build_tree(int node, int l, int r) {
    if (r == l+1) {
        sum[node] = maxlsum[node] = maxrsum[node] = maxisum[node] = a[l];
    } else {
        int m = l + (r - l + 1)/2;
        build_tree(2*node, l, m);
        build_tree(2*node+1, m, r);
        update(node);
    }
}
void query_rec(vector<int>& nodelist,
              int node, int tbegin, int tend, int abegin, int aend) {
    if (tbegin >= abegin && tend <= aend) {
        //fprintf(stderr, "DBG: %d %d\n", tbegin, tend);
        nodelist.push_back(node);
    } else {
        int mid = tbegin + (tend - tbegin + 1)/2;
        if (mid > abegin && tbegin < aend) {
            query_rec(nodelist, 2*node, tbegin, mid, abegin, aend);
        }
        if (tend > abegin && mid < aend) {
            query_rec(nodelist, 2*node+1, mid, tend, abegin, aend);
        }
    }
}
int query(int begin, int end) {
    vector<int> nodelist;
    query_rec(nodelist, 1, 0, N, begin, end);
    int best = -2e9;
    int tsum = -2e9;
    for (int i = 0; i < nodelist.size(); i++) {
        best = max(best, maxisum[nodelist[i]]);
        best = max(best, tsum + maxlsum[nodelist[i]]);
        tsum = max(maxrsum[nodelist[i]], tsum + sum[nodelist[i]]);
    }
    return best;
}
int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d", a + i);
    }
    build_tree(1, 0, N);
    scanf("%d", &M);
    for (int i = 0; i < M; i++) {
        int x, y;
        scanf("%d %d", &x, &y);
        printf("%d\n", query(x - 1, y));
    }
}
