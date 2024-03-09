// 2024-03-09
// This problem is the graph isomorphism problem in disguise, where a phrase
// "W1 W2" represents a directed edge W1 -> W2. The official solution uses a
// recursive backtracking approach. The code below does something much dumber
// that is basically guaranteed to work for random test data. When we relabel
// vertices, it permutes rows and columns at the same time: meaning that if we
// have a row in the adjacency matrix that's something like {0, 0, 1, 1}, then
// after the relabelling, that row might go somewhere else and might become a
// permutation such as {1, 0, 0, 1}. So if we can identify (u, v) where row u of
// the first adjacency matrix equals row v of the second, modulo ordering, and
// there is no other row in either matrix that has the same entries modulo
// ordering, we know that vertex u of the first graph corresponds to vertex v of
// the second graph. Repeat this until the two matrices are identical. In order
// to improve the chances that such a pair (u, v) will be found at each
// iteration, we replace the original adjacency matrices with their (2^30)th
// powers. In the case of an acyclic graph this will just zero them out, which
// is not helpful, so before we do the exponentiation, we add a back edge for
// each edge in the original graphs. This also has a risk of making the solution
// non-unique (consider the graph o -> o -> o, which would then become
// o <-> o <-> o) so we add *two* back edges for each forward edge (i.e.
// increase the corresponding entry in the adjacency matrix by 2). And voila, it
// gets accepted.
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;
constexpr int MOD = 1000000007;
vector<vector<int>> mulmod(const vector<vector<int>>& A,
                           const vector<vector<int>>& B) {
    const int N = A.size();
    vector<vector<int>> result(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            long long x = 0;
            for (int k = 0; k < N; k++) {
                x += ((long long)A[i][k]) * B[k][j];
                x %= MOD;
            }
            result[i][j] = x;
        }
    }
    return result;
}
void do_testcase(int E) {
    unordered_map<string, int> id1;
    unordered_map<string, int> id2;
    vector<string> word1;
    vector<string> word2;
    vector<vector<int>> adj1(25, vector<int>(25));
    vector<vector<int>> adj2(25, vector<int>(25));
    for (int i = 0; i < E; i++) {
        string w1, w2; cin >> w1 >> w2;
        int i1, i2;
        if (id1.count(w1)) {
            i1 = id1[w1];
        } else {
            word1.push_back(w1);
            i1 = id1[w1] = word1.size() - 1;
        }
        if (id1.count(w2)) {
            i2 = id1[w2];
        } else {
            word1.push_back(w2);
            i2 = id1[w2] = word1.size() - 1;
        }
        adj1[i1][i2] += 1;
        adj1[i2][i1] += 2;
    }
    for (int i = 0; i < E; i++) {
        string w1, w2; cin >> w1 >> w2;
        int i1, i2;
        if (id2.count(w1)) {
            i1 = id2[w1];
        } else {
            word2.push_back(w1);
            i1 = id2[w1] = word2.size() - 1;
        }
        if (id2.count(w2)) {
            i2 = id2[w2];
        } else {
            word2.push_back(w2);
            i2 = id2[w2] = word2.size() - 1;
        }
        adj2[i1][i2] += 1;
        adj2[i2][i1] += 2;
    }
    const int V = word1.size();
    adj1.resize(V);
    adj2.resize(V);
    for (int i = 0; i < V; i++) {
        adj1[i].resize(V);
        adj2[i].resize(V);
    }
    for (int i = 0; i < 30; i++) {
        adj1 = mulmod(adj1, adj1);
        adj2 = mulmod(adj2, adj2);
    }
    while (adj1 != adj2) {
        vector<pair<vector<int>, int>> rows1;
        vector<pair<vector<int>, int>> rows2;
        for (int i = 0; i < V; i++) {
            vector<int> row1 = adj1[i];
            sort(row1.begin(), row1.end());
            rows1.emplace_back(move(row1), i);
            vector<int> row2 = adj2[i];
            sort(row2.begin(), row2.end());
            rows2.emplace_back(move(row2), i);
        }
        sort(rows1.begin(), rows1.end());
        sort(rows2.begin(), rows2.end());
        // try to find a unique signature
        int i = 0;
        bool success = false;
        while (i < V) {
            int j = i + 1;
            while (j < V && rows1[i].first == rows1[j].first) ++j;
            if (j - i > 1 || rows1[i].second == rows2[i].second) {
                i = j;
                continue;
            }
            const int u = rows1[i].second;
            const int v = rows2[i].second;
            swap(adj1[u], adj1[v]);
            for (int k = 0; k < V; k++) {
                swap(adj1[k][u], adj1[k][v]);
            }
            swap(word1[u], word1[v]);
            success = true;
            break;
        }
        if (!success) throw;
    }
    vector<string> result;
    for (int i = 0; i < V; i++) {
        result.push_back(word1[i] + "/" + word2[i]);
    }
    sort(result.begin(), result.end());
    for (int i = 0; i < V; i++) {
        cout << result[i] << '\n';
    }
}
int main() {
    ios::sync_with_stdio(false);
    int E;
    bool first = true;
    for (;;) {
        cin >> E;
        if (E == 0) return 0;
        if (!first) {
            cout << '\n';
        }
        first = false;
        do_testcase(E);
    }
}
