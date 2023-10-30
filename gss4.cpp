// 2023-10-30
// The basic idea is that for each positive N <= 10^18 the sequence [N, sqrt(N),
// sqrt(sqrt(N)), ...] can have at most 6 elements that are greater than 1. So
// we build a segment tree where each leaf node contains those 6 elements and
// each internal node contains the sum of the array elements, the sum of the
// square roots of the array elements, etc. In order to apply the range update
// operation, we use lazy propagation. When querying, if an interval has had the
// square root operation applied more than 5 times, we know that the sum of an
// interval is simply the interval's length.
//
// The time limit for this problem is a bit strict. The comments hint at one
// particular optimization that suffices to get AC: avoid recursing into
// subtrees if it is known that all elements in the corresponding interval are
// already 1. We do this by not pushing down the 'lazy' value if it's already
// greater than 5.
#include <cstdio>
#include <cmath>
#include <vector>
#include <utility>
using namespace std;
long long isqrt(long long x) {
    return floor(sqrt((long double)x));
}
struct Node {
    int lazy;
    long long sum[6];
};
Node tree[400000];

void build_rec(int node, long long* begin, long long* end) {
    if (end == begin+1) {
        tree[node].lazy = 0;
        tree[node].sum[0] = *begin;
        for (int i = 1; i <= 5; i++) {
            tree[node].sum[i] = isqrt(tree[node].sum[i-1]);
        }
    } else {
        long long* mid = begin + (end-begin+1)/2;
        build_rec(2*node+1, begin, mid);
        build_rec(2*node+2, mid, end);
        tree[node].lazy = 0;
        for (int i = 0; i <= 5; i++) {
            tree[node].sum[i] =
                tree[2*node+1].sum[i] + tree[2*node+2].sum[i];
        }
    }
}
void update_range_rec(int node, int tbegin, int tend,
                      int abegin, int aend) {
    if (tbegin >= abegin && tend <= aend) {
        tree[node].lazy++;
        return;
    }
    int mid = tbegin + (tend - tbegin + 1)/2;
    if (tree[node].lazy) {
        if (tree[node].lazy > 5) {
            // Don't bother pushing it down; this entire subtree doesn't need
            // to be updated
            return;
        }
        for (int i = 0; i + tree[node].lazy <= 5; i++) {
            tree[node].sum[i] = tree[node].sum[i+tree[node].lazy];
        }
        for (int i = max(0, 6 - tree[node].lazy); i <= 5; i++) {
            tree[node].sum[i] = tend - tbegin;
        }
        tree[2*node+1].lazy += tree[node].lazy;
        tree[2*node+2].lazy += tree[node].lazy;
        tree[node].lazy = 0;
    }
    if (mid > abegin && tbegin < aend)
        update_range_rec(2*node+1, tbegin, mid, abegin, aend);
    if (tend > abegin && mid < aend)
        update_range_rec(2*node+2, mid, tend, abegin, aend);
    int lazy1 = tree[2*node+1].lazy;
    int lazy2 = tree[2*node+2].lazy;
    for (int i = 0; i <= 5; i++) {
        tree[node].sum[i] =
            (i+lazy1 <= 5 ? tree[2*node+1].sum[i+lazy1] : mid - tbegin) +
            (i+lazy2 <= 5 ? tree[2*node+2].sum[i+lazy2] : tend - mid);
    }
}
long long query_rec(int node, int tbegin, int tend, int abegin, int aend) {
    if (tree[node].lazy > 5) {
        // [abegin, aend) isn't necessarily contained within [tbegin, tend), bu
        // we do know the answer for the part of it that is contained
        return min(tend, aend) - max(tbegin, abegin);
    }
    if (tbegin >= abegin && tend <= aend) {
        return tree[node].sum[tree[node].lazy];
    }
    int mid = tbegin + (tend - tbegin + 1)/2;
    if (tree[node].lazy) {
        for (int i = 0; i + tree[node].lazy <= 5; i++) {
            tree[node].sum[i] = tree[node].sum[i+tree[node].lazy];
        }
        for (int i = max(0, 6 - tree[node].lazy); i <= 5; i++) {
            tree[node].sum[i] = tend - tbegin;
        }
        tree[2*node+1].lazy += tree[node].lazy;
        tree[2*node+2].lazy += tree[node].lazy;
        tree[node].lazy = 0;
    }
    long long res = 0;
    if (mid > abegin && tbegin < aend) 
        res += query_rec(2*node+1, tbegin, mid, abegin, aend);
    if (tend > abegin && mid < aend)
        res += query_rec(2*node+2, mid, tend, abegin, aend);
    return res;
}

long long a[100000];

int main() {
    int _cs = 0;
    for (;;) {
        _cs++;
        int N;
        if (scanf("%d", &N) < 1) return 0;
        for (int i = 0; i < N; i++) {
            scanf("%lld", a + i);
        }
        build_rec(0, a, a+N);
        int M; scanf("%d", &M);
        printf("Case #%d:\n", _cs);
        while (M--) {
            int i, x, y; scanf("%d %d %d", &i, &x, &y);
            if (x > y) swap(x, y); // stupid
            if (i == 0) {
                update_range_rec(0, 0, N, x-1, y);
            } else {
                printf("%lld\n", query_rec(0, 0, N, x-1, y));
            }
        }
        putchar('\n');
    }
}
