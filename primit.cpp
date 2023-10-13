// 2023-10-12
// The idea is to add the smallest possible number of edges to the graph so
// that it has a Eulerian path. The genetic code then corresponds to the
// Eulerian path (and the number of elements in the genetic code is 1 greater
// than the number of edges).
//
// First, note that if the graph has multiple connected components (considering
// each edge to be undirected) then each such component can be analyzed
// independently (we don't need to worry about connecting them to each other;
// just compute the answer for each component and take the sum at the end).
// Within each component, to create a Eulerian path, we repeatedly take some
// node whose outdegree exceeds its indegree, and some node whose indegree
// exceeds its outdegree, and add an edge going from the latter to the former.
// We will have to repeat this until there are at most two nodes left whose
// indegree and outdegree aren't equal (one of which must have outdegree equal
// to indegree plus one, and the other the reverse). We don't need to actually
// compute the result of this process; it's easy to determine how many steps it
// will take.
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <vector>
using namespace std;
void do_testcase() {
    int n; scanf("%d", &n);
    vector<vector<int>> uadj(1000);
    vector<int> vis(1000, 0);
    vector<int> indeg(1000, 0);
    vector<int> outdeg(1000, 0);
    vector<vector<int>> ucc;
    while (n--) {
        int l, r; scanf("%d %d", &l, &r); --l; --r;
        outdeg[l]++;
        indeg[r]++;
        uadj[l].push_back(r);
        uadj[r].push_back(l);
    }
    for (int i = 0; i < 1000; i++) {
        if (uadj[i].empty()) continue;
        if (vis[i]) continue;
        vector<int> component;
        // crappy simulated call stack (reduces memory usage)
        stack<pair<short, short>> S;
        S.emplace(i, 0);
        while (!S.empty()) {
            auto& p = S.top();
            const int v = p.first;
            short& j = p.second;
            vis[v] = 1;
            if (j == 0) {
                component.push_back(v);
            }
            while (j < uadj[v].size()) {
                if (!vis[uadj[v][j]]) {
                    S.emplace(uadj[v][j], 0);
                    j++;
                    goto end;
                } else {
                    j++;
                }
            }
            S.pop();
end:        ;
        }
        ucc.push_back(move(component));
    }
    int result = 0;
    for (const auto& component : ucc) {
        int delta = 0;
        int edges = 0;
        for (const auto node : component) {
            edges += outdeg[node];
            delta += abs(indeg[node] - outdeg[node]);
        }
        result += edges + max(delta / 2, 1);
    }
    printf("%d\n", result);
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
