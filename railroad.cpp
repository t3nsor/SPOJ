// 2024-02-26
// This problem is quite similar to TTABLE. We initially leave the source city
// at the given "earliest journey's starting time" and compute the earliest time
// by which we can reach the destination city using Dijkstra's algorithm, then
// successively decrement the time at which we leave from the source city and
// update arrival times at all other cities.
#include <algorithm>
#include <iostream>
#include <queue>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <utility>
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
struct QNode {
    int atime;
    int station;
    friend bool operator<(const QNode& n1, const QNode& n2) {
        return n1.atime > n2.atime;
    }
};
void do_testcase() {
    int V; cin >> V;
    vector<string> name(V);
    unordered_map<string, int> id;
    for (int i = 0; i < V; i++) {
        cin >> name[i];
        id[name[i]] = i;
    }
    vector<Station> stations(V);
    int L; cin >> L;  // number of train lines
    while (L--) {
        int S; cin >> S;  // stops on this line
        int last_station;
        int last_time;
        for (int i = 0; i < S; i++) {
            int t; string s; cin >> t >> s;
            const int station = id[s];
            if (i > 0) {
                stations[last_station].edges.push_back({last_time, t, station});
            }
            last_station = station;
            last_time = t;
        }
    }
    for (auto& s : stations) {
        s.last_edge_used = s.edges.size();
        s.last_atime = 1e6;
        sort(s.edges.begin(), s.edges.end(),
             [](const Edge& e1, const Edge& e2) {
                 return e1.dtime < e2.dtime;
             });
    }
    int t0; cin >> t0;
    string source_name, dest_name; cin >> source_name >> dest_name;
    const int source = id[source_name], dest = id[dest_name];
    int earliest_arrival = 1e6;
    int latest_departure;
    for (int dtime_index = stations[source].edges.size() - 1;
         dtime_index >= 0;
         --dtime_index) {
        const int dtime = stations[source].edges[dtime_index].dtime;
        if (dtime < t0) break;
        if (dtime == stations[source].last_atime) continue;
        priority_queue<QNode> Q;
        Q.push({dtime, source});
        while (!Q.empty()) {
            QNode node = Q.top();
            Q.pop();
            if (stations[node.station].last_atime <= node.atime) continue;
            stations[node.station].last_atime = node.atime;
            if (node.station == dest && node.atime < earliest_arrival) {
                earliest_arrival = node.atime;
                latest_departure = dtime;
                break;
            }
            for (int& i = stations[node.station].last_edge_used;
                 i > 0 &&
                   stations[node.station].edges[i - 1].dtime >= node.atime;
                 --i) {
                Q.push({stations[node.station].edges[i - 1].atime,
                        stations[node.station].edges[i - 1].dest});
            }
        }
    }
    if (earliest_arrival == 1e6) {
        cout << "No connection\n";
    } else {
        char buf[5];
        sprintf(buf, "%04d", latest_departure);
        cout << "Departure " << buf << ' ' << source_name << '\n';
        sprintf(buf, "%04d", earliest_arrival);
        cout << "Arrival   " << buf << ' ' << dest_name << '\n';
    }
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    for (int i = 1; i <= T; i++) {
        cout << "Scenario " << i << '\n';
        do_testcase();
        cout << '\n';
    }
}
