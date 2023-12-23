// 2023-12-23
#include <algorithm>
#include <stdio.h>
#include <vector>
using namespace std;
struct st {
    int v;
    int cap;
    int back;
};
// Brian's implementation of the relabel-to-front algorithm with gap heuristic.
long long get_max_flow(vector<vector<st>>&& adj, int s, int t) {
    struct ListNode {
        int prev;
        int next;
    };
    const int V = adj.size();
    vector<long long> excess(V, 0);
    vector<int> height(V, 0);
    vector<int> heightcnt(2*V + 1, 0);
    vector<ListNode> L(V + 1);
    L[V].prev = V;
    L[V].next = V;
    for (int i = 0; i < V; i++) {
        if (i == s || i == t) continue;
        L[i].next = V;
        L[i].prev = L[V].prev;
        L[L[V].prev].next = i;
        L[V].prev = i;
    }
    height[s] = V;
    heightcnt[V] = 1;
    heightcnt[0] = V - 1;
    for (auto& edge : adj[s]) {
        excess[edge.v] += edge.cap;
        adj[edge.v][edge.back].cap = edge.cap;
        edge.cap = 0;
    }
    auto move_to_front = [&L,V](int u) {
        L[L[u].prev].next = L[u].next;
        L[L[u].next].prev = L[u].prev;
        L[u].next = L[V].next;
        L[u].prev = V;
        L[L[V].next].prev = u;
        L[V].next = u;
    };
    int u = L[V].next;
    while (u != V) {
        bool relabelled = false;
        while (excess[u] > 0) {
            int best = 1e9;
            for (auto& edge : adj[u]) {
                if (!edge.cap) continue;
                if (height[u] == height[edge.v] + 1) {
                    const int x = min<long long>(excess[u], edge.cap);
                    excess[u] -= x;
                    excess[edge.v] += x;
                    edge.cap -= x;
                    adj[edge.v][edge.back].cap += x;
                    if (excess[u] == 0) break;
                } else {
                    best = min(best, height[edge.v]);
                }
            }
            if (excess[u] > 0) {
                relabelled = true;
                const int old_height = height[u];
                height[u] = best + 1;
                heightcnt[best + 1]++;
                if (0 == --heightcnt[old_height] && old_height < V) {
                    for (int i = 0; i < V; i++) {
                        if (i != s && i != t &&
                            height[i] > old_height && height[i] <= V) {
                            heightcnt[height[i]]--;
                            height[i] = V + 1;
                            move_to_front(i);
                        }
                    }
                }
                move_to_front(u);
            }
        }
        if (!relabelled) {
            u = L[u].next;
        }
    }
    return excess[t];
}
int main() {
    int N, M; scanf("%d %d", &N, &M);
    vector<vector<st>> adj(N);
    while (M--) {
        int A, B, C;
        scanf("%d %d %d", &A, &B, &C); --A; --B;
        if (A == B) continue;
        adj[A].push_back(st{B, C, (int)adj[B].size()});
        adj[B].push_back(st{A, C, (int)adj[A].size() - 1});
    }
    printf("%lld\n", get_max_flow(move(adj), 0, N - 1));
    return 0;
}
