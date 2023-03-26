// 2023-03-25
// This problem has a surprisingly small number of accepted users on SPOJ, but
// the algorithm isn't complicated. Initially, assume we leave station 1 at
// 23:59 and use Dijkstra's to calculate the earliest time we can reach each
// station. Then, suppose we instead leave station 1 at 23:58 and again run
// Dijkstra's to update the earliest arrival time for each station. Each station
// has a list of departure; we keep track of the earliest departure from each
// station that is possible to catch. Each time we update the arrival time for a
// station, we only process those edges that are newly possible to catch (i.e.
// departure time is greater than or equal to the arrival time to the current
// station, but strictly less than the previous best arrival time to the current
// station). In this way we ensure that each edge is only used once during the
// entire test case.
#include <cctype>
#include <cstdio>
#include <queue>
#include <vector>
using namespace std;
struct Edge {
    int dtime;
    int atime;
    int dest;
};
struct Station {
    int last_edge_used;
    int last_atime;
    vector<Edge> edges;
};
int read_time() {
    char c;
    do {
        c = getchar();
    } while (isspace(c));
    int result = c - '0';
    result = 10 * result + getchar() - '0';
    getchar();  // colon
    result = 6 * result + getchar() - '0';
    return 10 * result + getchar() - '0';
}
void write_time(int time) {
    putchar(time / 600 + '0');
    time %= 600;
    putchar(time / 60 + '0');
    time %= 60;
    putchar(':');
    putchar(time / 10 + '0');
    putchar(time % 10 + '0');
}
void dijkstra(vector<Station>& stations, int stime) {
    priority_queue<pair<int, int>,
                   vector<pair<int, int>>,
                   greater<pair<int, int>>> Q;
    Q.emplace(stime, 0);
    while (!Q.empty()) {
        const auto p = Q.top();
        Q.pop();
        const int u = p.second;
        const int time = p.first;
        if (time >= stations[u].last_atime) continue;
        stations[u].last_atime = time;
        int& idx = stations[u].last_edge_used;
        while (idx > 0 && stations[u].edges[idx-1].dtime >= time) {
            --idx;
            const auto& edge = stations[u].edges[idx];
            const int v = edge.dest;
            if (edge.atime < stations[v].last_atime) {
                Q.emplace(edge.atime, v);
            }
        }
    }
}
void do_testcase() {
    int V; scanf("%d", &V);
    vector<Station> stations(V);
    for (int i = 0; i < V; i++) {
        int deg; scanf("%d", &deg);
        stations[i].last_edge_used = deg;
        stations[i].last_atime = 8640;
        while (deg--) {
            stations[i].edges.emplace_back();
            Edge& e = stations[i].edges.back();
            e.dtime = read_time();
            e.atime = read_time();
            scanf("%d", &e.dest); --e.dest;
        }
    }
    vector<Edge> result;
    Edge sentinel;
    sentinel.dtime = 8640;
    sentinel.atime = 8640;
    sentinel.dest = -1;
    result.push_back(sentinel);
    for (int i = 8639; i >= 0; i--) {
        dijkstra(stations, i);
        if (stations[V-1].last_atime < result.back().atime) {
            Edge optimal;
            optimal.dtime = i;
            optimal.atime = stations[V-1].last_atime;
            optimal.dest = -1;
            result.push_back(optimal);
        }
    }
    printf("%d\n", static_cast<int>(result.size() - 1));
    for (int i = result.size() - 1; i >= 1; i--) {
        write_time(result[i].dtime);
        putchar(' ');
        write_time(result[i].atime);
        putchar('\n');
    }
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
