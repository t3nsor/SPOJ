// 2022-09-15
// We are asked to find a hamiltonian cycle in a graph with special constraints.
// The graph is certainly planar, but it turns out that finding a hamiltonian
// cycle is NP-hard even in the special case of planar graphs. The graph we are
// given is more than just planar: it is guaranteed to have a planar embedding
// where all vertices are placed at the vertices of a convex polygon.
//
// Our method is based on successively deleting edges in order to reduce the
// problem to a simpler one. First, note that even if the polygon is fully
// triangulated, there must remain at least one vertex of degree 2 (i.e., with
// exactly 2 potential neighbours given in the input). Whenever we see a vertex
// of degree 2, we know who its actual neighbours are; this implies that its
// neighbours also have it as an actual neighbour. For that matter, the same
// holds for any vertex that has 1 known neighbour and 1 potential neighbour.
// In each case, processing the vertex `v` may cause one of its neighbours `n`
// to have both of its actual neighbours known. That implies that any remaining
// potential neighbours of `n` (call such a vertex `p`) is not a neighbour; the
// edge `n`--`p` is a diagonal. That means `n` can be removed from the potential
// neighbour set of `p`. Finally, there is also a case where an edge is known
// to be a diagonal because it connects two endpoints of a currently known chain
// (and those two endpoints also have other potential neighbours besides each
// other); this is illustrated by the following case:
//
// x---x---x
// |  / \ / \
// x-x---x---x
//
// Here, the two leftmost nodes start out with only two potential neighbours
// each, so their actual neighbours are known; they belong to a chain with each
// other and with their two other neighbours (the middle node on the top and
// the second from the left on the bottom). We can also form a chain of length
// 3 centered on the lower-right node, but then we are stuck unless we observe
// that, for each of these two chains, there is an edge connecting its two
// endpoints; such an edge must be a diagonal.
//
// At any given point, at least one of the above rules applies, allowing us to
// either identify an edge as part of the hamiltonian cycle or as a diagonal
// (and thus removing it from the potential neighbour set of each of its
// endpoints), eventually resulting in successful termination.
//
// This algorithm should run in linear time, although the fact that it takes
// 0.20s on SPOJ makes me wonder whether there's something wrong with my
// implementation. The main loop (`while (!q.empty()) { ... }`) can only run
// O(n) times, because we only add O(n) elements initially and each subsequent
// insertion that occurs within the main loop is paired with a deletion (and
// only O(n) total deletions can occur before all edges are exhausted). Likewise
// the inner loop can only run O(n) times in total since it deletes an edge on
// each iteration.

#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
using namespace std;
struct Chain {
    int front;
    int back;
    int other(int front_or_back) const {
        return front_or_back == front ? back : front;
    }
};
void merge(vector<Chain*>& chains, int to_vertex, int from_vertex) {
    auto& to_chain = chains[to_vertex];
    auto& from_chain = chains[from_vertex];
    if (to_chain->back == to_vertex) {
        swap(to_chain->front, to_chain->back);
    }
    if (from_chain->front == from_vertex) {
        swap(from_chain->front, from_chain->back);
    }
    // to_vertex == to_chain->front && from_vertex == from_chain->back
    // link the two chains, from_vertex---to_vertex
    to_chain->front = from_chain->front;
    chains[to_chain->front] = to_chain;
}
void do_testcase() {
    int n, m; cin >> n >> m;
    // neighbour sets
    vector<unordered_set<int>> potential(n);
    vector<unordered_set<int>> actual(n);
    vector<Chain> chains(n);
    for (int i = 0; i < m + n; i++) {
        int a, b; cin >> a >> b; a--; b--;
        potential[a].insert(b);
        potential[b].insert(a);
    }
    vector<Chain*> chain_of(n);
    queue<int> q;  // can also be a stack or whatever
    for (int i = 0; i < n; i++) {
        chains[i].front = i;
        chains[i].back = i;
        chain_of[i] = &chains[i];
        if (potential[i].size() == 2) {
            q.push(i);
        }
    }
    while (!q.empty()) {
        const int i = q.front();
        q.pop();
        if (actual[i].size() == 2) continue;
        if (actual[i].size() + potential[i].size() == 2) {
            const int nbr = *potential[i].begin();
            merge(chain_of, i, nbr);
            q.push(chain_of[i]->front);
            q.push(chain_of[i]->back);
            actual[i].insert(nbr);
            actual[nbr].insert(i);
            potential[nbr].erase(i);
            potential[i].erase(nbr);
            if (actual[nbr].size() == 2 && !potential[nbr].empty()) {
                for (const auto p : potential[nbr]) {
                    potential[p].erase(nbr);
                    q.push(p);
                }
                potential[nbr].clear();
            }
        } else if (actual[i].size() == 1) {
            const int other = chain_of[i]->other(i);
            if (actual[other].size() == 1 &&
                potential[other].size() > 1 &&
                potential[i].count(other)) {
                potential[i].erase(other);
                potential[other].erase(i);
                q.push(i);
                q.push(other);
            }
        }
    }
    vector<int> result = {0, *actual[0].begin()};
    for (int i = 2; i < n; i++) {
        const int last = result[i-1];
        const int secondlast = result[i-2];
        for (const auto nbr : actual[last]) {
            if (nbr != secondlast) {
                result.push_back(nbr);
                break;
            }
        }
    }
    if (result.back() < result[1]) {
        reverse(result.begin() + 1, result.end());
    }
    cout << result[0] + 1;
    for (int i = 1; i < n; i++) {
        cout << ' ' << result[i] + 1;
    }
    cout << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int d; cin >> d;
    while (d--) do_testcase();
}
