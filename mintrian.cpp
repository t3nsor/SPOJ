// 2026-08-04
// This is a "proof of concept" implementation of the standard algorithm for
// detecting chordal graphs in linear time (Rose et al., 1976; Habib et al.,
// 2000) without using hash tables.  I don't know how bad the performance would
// actually be with hash tables; I just felt that since they're not supposed to
// be needed, it's good to know how to not use them.
//
// The problem statement doesn't state bounds; it seems we are just meant to
// assume that they are "reasonable" if you have a linear time algorithm.  In
// the actual test data there were no cases with V greater than 1000 or E
// greater than 80,000, but you probably shouldn't rely on that.  To give you
// some idea of how much constant optimization might be required (or not), the
// sum of E across all test cases was around 530,000.
#include <list>
#include <stdio.h>
#include <vector>
using namespace std;
// A subset during the LexBFS step remembers its "last split time" so that we
// don't split it twice during the same iteration of the outer loop.
struct Subset : vector<int> {
    int split_time;
};
bool do_testcase() {
    int V, E; scanf(" n=%d,m=%d", &V, &E);
    vector<vector<int>> adj(V);
    while (E--) {
        int u, v; scanf(" {%d,%d}", &u, &v);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    list<Subset> subset_list;
    subset_list.emplace_back();  // split_time = 0
    auto it = subset_list.begin();
    vector<decltype(it)> subset_for(V, it);
    for (int i = 0; i < V; i++) it->push_back(i);
    vector<int> lex;
    vector<int> rank(V, -1);
    for (int i = 0; i < V; i++) {
retry:
        while (it->empty()) ++it;
        const int u = it->back();
        it->pop_back();
        if (subset_for[u] != it) {
            // this vertex was already moved to an earlier list (see below)
            goto retry;
        }
        lex.push_back(u);
        rank[u] = i;
        for (const auto v : adj[u]) {
            if (rank[v] >= 0) continue;
            auto sit = subset_for[v];
            if (sit->split_time < lex.size()) {
                sit->split_time = lex.size();
                subset_list.emplace(sit);
                if (sit == it) --it;
            }
            --sit;
            subset_for[v] = sit;
            // To avoid hash tables, we don't actually attempt to delete `v`
            // from the original subset.  Instead, we have updated `subset_for`
            // above to indicate which subset `v` actually belongs to; when we
            // later encounter `v` in its original subset, we will ignore it
            // then.
            sit->push_back(v);
        }
    }
    // For each vertex, make a list of its neighbors that occur earlier in the
    // lexicographical ordering, sorted by the position of such neighbors in the
    // lexicographical ordering.  (Note that after this point, we don't need the
    // original adjlists anymore.)
    vector<vector<int>> nlist(V);
    for (int i = 0; i < V; i++) {
        const auto u = lex[i];
        for (const auto v : adj[u]) {
            if (rank[v] > rank[u]) nlist[v].push_back(u);
        }
    }
    // The "parent" of a vertex is its neighbor that occurs before it in the
    // lexicographical ordering but as late as possible.  For each vertex, we
    // make a list of vertices that have it as its parent.
    vector<vector<int>> children(V);
    for (int v = 0; v < V; v++) {
        if (!nlist[v].empty()) children[nlist[v].back()].push_back(v);
    }
    // For a given vertex u, let its children be v_1, ..., v_k.  If any v_i has
    // an earlier neighbor other than u, which is not also a neighbor of u, then
    // the graph is not chordal.  If we never determine using this test that the
    // graph is not chordal, then it is.
    vector<vector<int>> expect(V);
    vector<int> nextidx(V);
    for (int u = 0; u < V; u++) {
        for (const auto v : children[u]) {
            nextidx[v] = nlist[v].size() - 2;
            if (nextidx[v] >= 0) expect[nlist[v][nextidx[v]]].push_back(v);
        }
        for (int i = nlist[u].size() - 1; i >= 0; i--) {
            for (const auto v : expect[nlist[u][i]]) {
                if (--nextidx[v] >= 0) {
                    expect[nlist[v][nextidx[v]]].push_back(v);
                }
            }
            expect[nlist[u][i]].clear();
        }
        for (const auto v : children[u]) if (nextidx[v] >= 0) return false;
    }
    return true;
}
int main() {
    int t; scanf("%d", &t); while (t--) puts(do_testcase() ? "YES" : "NO");
}
