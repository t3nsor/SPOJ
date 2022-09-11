// 2022-09-11
// A "less naive" solution compared to `bus-1.cpp`. We consider each piece of
// news independently, finding the minimum time required for all drivers to know
// that piece of news. To find the minimum time for a given piece of news, we
// use a Dijkstra-like approach: repeatedly select the driver that doesn't know
// the news yet that will find out the news earliest out of all such drivers. To
// update the times, we need a way of determining, for a given time `t` and a
// given pair of drivers (where the first has just learned the news, and the
// second doesn't know it yet) the earliest time >= `t` when those two drivers
// will meet. This can be done using the Chinese remainder theorem. We
// precompute all meeting times for all pairs of drivers before we begin the
// main loop over pieces of news. (Since the number of drivers is at most 49, we
// find the next driver just by looping, but we could easily rewrite this so
// that it uses a heap. I'm not sure if that would improve the runtime much.)
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

// return a % b (positive value)
int mod(int a, int b) {
    return ((a%b)+b)%b;
}

// returns d = gcd(a,b); finds x,y such that d = ax + by
int extended_euclid(int a, int b, int &x, int &y) {  
    int xx = y = 0;
    int yy = x = 1;
    while (b) {
	int q = a/b;
	int t = b; b = a%b; a = t;
	t = xx; xx = x-q*xx; x = t;
	t = yy; yy = y-q*yy; y = t;
    }
    return a;
}

int solve_one(int n, int start, int (&lcm)[50][50],
              vector<int> (&meet_times)[50][50]) {
    vector<int> vis(50, 0);
    vector<int> when(50, INT_MAX);
    int num_vis = 0;
    when[start] = 0;
    for (;;) {
        int cur = -1;
        for (int i = 0; i < n; i++) {
            if (!vis[i] && (cur == -1 || when[i] < when[cur])) {
                cur = i;
            }
        }
        if (when[cur] == INT_MAX) return INT_MAX;
        const int time = when[cur];
        vis[cur] = 1;
        if (n == ++num_vis) return time;
        // when will we meet each of the other buses?
        for (int next = 0; next < n; next++) {
            if (vis[next]) continue;
            if (meet_times[cur][next].empty()) continue;
            const int tmod = time % lcm[cur][next];
            int next_mod = meet_times[cur][next][0];
            for (const int meet_time : meet_times[cur][next]) {
                if (meet_time >= tmod) {
                    next_mod = meet_time;
                    break;
                }
            }
            const int next_time = time + mod(next_mod - tmod, lcm[cur][next]);
            when[next] = min(when[next], next_time);
        }
    }
}

void do_testcase() {
    int n; cin >> n;
    if (n == 0) exit(0);
    vector<int> routes[50];
    for (int i = 0; i < n; i++) {
        int s; cin >> s;
        while (s--) {
            int x; cin >> x;
            routes[i].push_back(x);
        }
    }
    // for every pair of lines,
    // determine all times when the buses on those lines will meet
    int lcm[50][50];
    unordered_multimap<int, int> stop_to_index[50];
    vector<int> meet_times[50][50];
    for (int i = 0; i < n; i++) {
        for (size_t j = 0; j < routes[i].size(); j++) {
            // This assertion gets triggered :(
            // I think it's sort of misleading to call a line "circular" if it
            // can go like 0 -> 1 -> 0 -> 2 -> ...
            // if (stop_to_index[i].count(routes[i][j])) {
            //     throw runtime_error("duplicate stops on a route");
            // }
            stop_to_index[i].emplace(routes[i][j], j);
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            const int a = routes[i].size();
            const int b = routes[j].size();
            int x, y;
            const int gcd = extended_euclid(a, b, x, y);
            lcm[i][j] = a * b / gcd;
            auto& mt = meet_times[i][j];
            for (int k = 0; k < a; k++) {
                const auto p = stop_to_index[j].equal_range(routes[i][k]);
                for (auto it = p.first; it != p.second; ++it) {
                    const int m = it->second;
                    if ((k - m) % gcd) continue;
                    mt.push_back(mod(x*m*a + y*k*b, a*b) / gcd);
                }
            }
            sort(mt.begin(), mt.end());
            mt.erase(unique(mt.begin(), mt.end()), mt.end());
        }
    }

    int result = 0;
    for (int i = 0; i < n; i++) {
        // determine time needed for all drivers to know news #i
        result = max(result, solve_one(n, i, lcm, meet_times));
    }

    if (result == INT_MAX) {
        cout << "NEVER\n";
    } else {
        cout << result << '\n';
    }
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) do_testcase();
}
