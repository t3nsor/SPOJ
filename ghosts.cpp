// 2017-07-08
// See G.F. Italiano, "Amortized efficiency of a path retrieval data
// structure", Theoretical Computer Science 48 (1986): 273-281
#include <cstdio>
#include <deque>
#include <vector>
using namespace std;
struct Node {
    int vertex;
    Node* parent = nullptr;
    vector<Node*> children;
};
struct Graph {
    Graph(int n) : n(n), pool(n), index(n, vector<Node*>(n)) {
        for (int i = 0; i < n; i++) {
            pool[i].vertex = i;
            index[i][i] = &pool[i];
        }
    }
    bool is_reachable(int i, int j) { return index[i][j]; }
    void add(int i, int j) {
        if (index[i][j]) return;
        for (int x = 0; x < n; x++) {
            if (index[x][i] && !index[x][j]) {
                meld(x, j, i, j);
            }
        }
    }
    void meld(int x, int j, int u, int v) {
        pool.emplace_back();
        Node* new_node = &pool.back();
        new_node->vertex = v;
        new_node->parent = index[x][u];
        index[x][v] = new_node;
        index[x][u]->children.push_back(new_node);
        for (const Node* w : index[j][v]->children) {
            if (!index[x][w->vertex]) meld(x, j, v, w->vertex);
        }
    }
    int n;
    deque<Node> pool;
    vector<vector<Node*>> index;
};
int main() {
    int K, T; scanf("%d %d", &K, &T);
    Graph g(K);
    while (T--) {
        int a, b; scanf("%d %d", &a, &b);
        if (g.is_reachable(b - 1, a - 1)) printf("%d %d\n", a, b);
        else g.add(a - 1, b - 1);
    }
    puts("0 0");
}
