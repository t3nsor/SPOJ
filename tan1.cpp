// 2026-06-18
// The problem statement is unclear when it says "(A, B, C) and (B, C, A) are
// supposed to be counted once".  The intent is that an unordered triple is
// counted once: (A, B, C) and any permutation of (A, B, C) should not both be
// counted.
//
// The first step, obviously, is to determine when a sequence A is good
// (winnable).  The powers of 5 modulo 8 are 1, 5, 1, 5, ..., so S is given by
// adding the elements of A, where half of them (rounded down) are multiplied
// by 5.  Equivalently, we add all the elements of A and then, for half of them
// (rounded down) we add them 4 times over.  In doing so, odd values become 4
// and even values become 0.  So A can be good only if its sum (mod 8) is 3 or
// 7.  If the sum is 3, A cannot be good if its length is 4n+3 and all its
// elements are odd; similarly, if the sum is 7, A cannot be good if its length
// is 4n+1 and all its elements are odd; I call these the "exceptional bad
// cases".  I believe there are no other exceptional bad cases but the proof
// isn't immediately obvious since in reality we cannot choose an arbitrary half
// of elements to multiply by 5 (e.g., 11551 is possible but 51115 is not).  I
// just assumed that it's true without proof, and checked it via brute force for
// all binary sequences up to length 17.
//
// So what we will need to do is compute, for each directed edge (u, v), the
// number of paths that start at v, do not pass through u (i.e. extend "away
// from" u), and have sum equal to 0, ..., 7 (mod 8).  Furthermore, we need a
// breakdown of each of these 8 buckets into all odd and not all odd, and the
// all odd bucket must itself be broken down by number of nodes (mod 4).  This
// information will allow us to compute the number of ways to use each node u
// as the middle node B for one of the combinations (A, B, C) that we are asked
// to count, or as the "root" at which the paths from A to B, B to C, and C to A
// all intersect.  This is the easy part.
//
// The hard part is actually using this information after we've computed it.
// The time limit is very strict.  The code below uses the following strategy.
// First, notice that a combination of sum and path length (for an all-odd path)
// is possible only when the parities match (e.g. we cannot have an all-odd path
// of length 3, whose sum is 4).  Then, also observe that a path of length l and
// sum s is equivalent to a path of length l + 4 and sum s + 2 (i.e. replacing
// one by the other never affects whether the path, or any path containing it,
// is good).  That means there are only 12 values to consider for each directed
// edge.  In fact, we can say that every path falls into exactly one of 12
// buckets.  Call this the path's "characteristic".  So for each directed edge
// u-v we have an array that gives the number of times each characteristic
// occurs among those paths that contain v and do not contain u.
//
// For each node u, we can imagine combining these paths as follows:
// (1) for two paths p1, p2 that start at distinct neighbors of u, call them v1,
//     v2, consider whether the paths u-p1, u-p2, and p1-u-p2 are all good or
//     all bad: this can be done using a lookup table (looking up the label of u
//     and the characteristics of p1, p2);
// (2) for three paths p1, p2, p3 that start at distinct v1, v2, v3, consider
//     whether the paths p1-u-p2, p2-u-p3, p3-u-p1 are all good or all bad,
//     again using a lookup table.
// Effectively, then, it boils down to computing certain sums, e.g. for (2) we
// need to find the sum A[q][i][j][k]*x[i]*y[j]*z[k] where q is the label of u,
// i, j, k loop over all possible characteristics, x, y, and z are the counts
// for distinct neighbors of u, and A is the lookup table, so A[q][i][j][k] is 1
// if the paths would be all good or all bad, and 0 otherwise.  Note that A
// should be precomputed.  We also precompute all "transitions", that is, the
// resulting characteristic of a path after a node with a particular label has
// been appended to it (in terms of the label and the original characteristic).
//
// To speed up the inner loop, we replace A (which is a 0-1 matrix) with a
// "compressed" matrix whose elements are bitmasks (i.e. integers whose bits
// correspond to bits from the original A).  But we can't compress too much,
// because each such bitmask tells us to sum elements of `z` (with indices at
// which the bitmask has a 1) so we need to precompute all such sums before we
// start the inner loop.  This solution compresses by a factor of 4, so that
// we have to precompute 3 sets of 16 combinations each.  We also need further
// constant optimizations to pass, which help in the case where a node's degree
// is small (see below).  And of course we avoid nested vectors in order to
// maximize cache locality.  Ultimately, this was a nasty constant optimization
// problem.  I do find it interesting that there are accepted solutions with
// time less than 0.1s.  I imagine they have some clever tricks that I'm not
// able to figure out.

#include <array>
#include <stdio.h>
#include <string.h>
#include <utility>
#include <vector>
using namespace std;

using LL = long long;

pair<int, int> unpack(int p) {
    if (p < 4) return make_pair(p, -1);
    else {
        const int t = (p - 4) / 2;
        const int l = 2*(p % 2) + (t % 2);
        return make_pair(t, l);
    }
}

int combos[8][13][12][3];

bool good(int label, initializer_list<int> a) {
    const auto p1 = unpack(a.begin()[0]);
    const int t1 = p1.first, len1 = p1.second;

    int tot = label + t1;
    int len = label % 2 == 1 && len1 >= 0 ? len1 + 1 : -1;

    if (a.size() > 1) {
        const auto p2 = unpack(a.begin()[1]);
        const int t2 = p2.first, len2 = p2.second;
        tot += t2;
        len = len >= 0 && len2 >= 0 ? len + len2 : -1;
    }
    tot %= 8; len %= 4;
    
    return (tot == 3 && len != 3) || (tot == 7 && len != 1);
}

void make_combos() {
    for (int l = 0; l < 8; l++) {
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 12; j++) {
                int s = good(l, {i}) + good(l, {j}) + good(l, {i, j});
                combos[l][12][i][j / 4] |= (s == 0 || s == 3) << (j % 4);
                for (int k = 0; k < 12; k++) {
                    int s = good(l, {i, j}) + good(l, {j, k}) + good(l, {k, i});
                    combos[l][k][i][j / 4] |= (s == 0 || s == 3) << (j % 4);
                }
            }
        }
    }
}

struct Counts : array<int, 12> { };

int label[100000];
int deg[100000];
int start[100000];
int nbr[200000];
Counts counts[200000];

LL trans[8][12];

void make_trans() {
    for (int label = 0; label < 8; label++) {
        for (int cur = 0; cur < 12; cur++) {
            if (cur < 4) {
                trans[label][cur] = (cur + label) % 4;
            } else {
                int s = (cur - 4) / 2;
                int l = 2*(cur % 2) + (s % 2);
                l++;
                int label2 = label;
                if (label2 >= 4) {
                    label2 -= 4;
                    l += 2;
                }
                s += label2;
                if (s >= 4) { s -= 4; l += 2; }
                l %= 4;
                if (label % 2) {
                    trans[label][cur] = 4 + 2*s + l/2;
                } else {
                    trans[label][cur] = s;
                }
            }
        }
    }
}

void dfs1(int parent, int u) {
    for (int i = 0; i < deg[u]; i++) {
        const auto v = nbr[start[u] + i];
        if (v == parent) continue;
        const auto vval = label[v];
        dfs1(u, v);
        auto& uvcnt = counts[start[u] + i];
        for (int j = 0; j < deg[v]; j++) {
            const auto w = nbr[start[v] + j];
            if (w == u) continue;
            const auto& vwcnt = counts[start[v] + j];
            for (int k = 0; k < 12; k++) {
                uvcnt[trans[vval][k]] += vwcnt[k];
            }
        }
        if (vval % 2) {
            if (vval > 4) {
                uvcnt[5 + 2*(vval - 4)]++;
            } else {
                uvcnt[4 + 2*vval]++;
            }
        } else uvcnt[vval % 4]++;
    }
}

LL dfs2(int parent, int u) {
    LL local_result = 0;
    LL rec_result = 0;
    Counts mytotal {};  // zeroed
    for (int i = 0; i < deg[u]; i++) {
        // includes parent
        for (int j = 0; j < 12; j++) mytotal[j] += counts[start[u] + i][j];
    }
    LL cnt2[13][12] = {0};
    for (int i = 0; i < deg[u]; i++) {
        const auto uval = label[u];
        const auto v = nbr[start[u] + i];
        const Counts& uvcnt = counts[start[u] + i];
        int mask1[16] = {0}, mask2[16] = {0}, mask3[16] = {0};
        if (i >= 1) {
            // note that for i == 0, the cnt2 matrix is zero, so there's no
            // point in computing the masks
            for (int j = 1; j < 16; j++) {
                const int k = __builtin_ctz(j);
                mask1[j] = mask1[j - (1 << k)] + uvcnt[k];
                mask2[j] = mask2[j - (1 << k)] + uvcnt[k + 4];
                mask3[j] = mask3[j - (1 << k)] + uvcnt[k + 8];
            }
        }
        if (i >= 2) {
            for (int j = 0; j < 13; j++) {
                for (int k = 0; k < 12; k++) {
                    local_result += cnt2[j][k] * mask1[combos[uval][j][k][0]];
                    local_result += cnt2[j][k] * mask2[combos[uval][j][k][1]];
                    local_result += cnt2[j][k] * mask3[combos[uval][j][k][2]];
                }
            }
        } else if (i == 1) {
            // If we haven't yet seen two branches (before the current one),
            // then the entries of cnt2 with j != 12 are zero, so we skip them.
            // (And if we haven't yet seen one branch, then all entries of cnt2
            // are zero, so we skip this single loop as well.)
            for (int k = 0; k < 12; k++) {
                local_result += cnt2[12][k] * mask1[combos[uval][12][k][0]];
                local_result += cnt2[12][k] * mask2[combos[uval][12][k][1]];
                local_result += cnt2[12][k] * mask3[combos[uval][12][k][2]];
            }
        }
        if (i + 1 < deg[u]) {
            // for the final neighbor, no point in updating `cnt2` since it
            // won't be used again
            if (i >= 1) {
                // and if this is the first branch, all entries of cnt2 are zero
                // so we can skip this double loop
                for (int j = 0; j < 12; j++) {
                    for (int k = 0; k < 12; k++) {
                        cnt2[j][k] += uvcnt[j]*cnt2[12][k];
                    }
                }
            }
            for (int j = 0; j < 12; j++) cnt2[12][j] += uvcnt[j];
        }
        // below we will compute the counts for the directed edge v-u, so v
        // should be a child (and not parent)
        if (v == parent) continue;
        int pidx;
        for (int j = 0; j < deg[v]; j++) {
            if (nbr[start[v] + j]  == u) { pidx = j; break; }
        }
        Counts& vucnt = counts[start[v] + pidx];
        for (int j = 0; j < 12; j++) {
            vucnt[trans[uval][j]] += mytotal[j] - uvcnt[j];
        }
        if (uval % 2) {
            if (uval > 4) {
                vucnt[5 + 2*(uval - 4)]++;
            } else {
                vucnt[4 + 2*uval]++;
            }
        } else vucnt[uval % 4]++;
        rec_result += dfs2(u, v);
    }
    return local_result + rec_result;
}

int get() {
    int result = 0;
    int c;
    do {
        c = getchar_unlocked();
    } while (c <= 32);
    do {
        result = 10*result + (c - '0');
        c = getchar_unlocked();
    } while (c > 32);
    return result;
}

void do_testcase() {
    int V; scanf("%d", &V);
    memset(deg, 0, V*sizeof(int));
    vector<pair<int, int>> edges;
    int root;
    for (int i = 0; i < V; i++) {
        int p = get() - 1;
        label[i] = get() % 8;
        if (p == -1) {
            root = i;
        } else {
            edges.emplace_back(i, p);
            deg[i]++;
            deg[p]++;
        }
    }
    memset(counts, 0, sizeof(Counts)*(2*V - 2));
    for (int i = 1; i < V; i++) start[i] = start[i - 1] + deg[i - 1];
    vector<int> seen(V, 0);
    for (const auto& edge : edges) {
        const int u = edge.first;
        const int v = edge.second;
        nbr[start[u] + seen[u]++] = v;
        nbr[start[v] + seen[v]++] = u;
    }
    edges.clear();
    seen.clear();

    dfs1(-1, root);
    const LL result = dfs2(-1, root);
    printf("%lld\n", result);
}

int main() {
    make_combos();
    make_trans();
    int t = get(); while (t--) do_testcase();
}
