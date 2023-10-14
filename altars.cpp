// 2023-10-13
// This is basically just a line sweep, but we use purely integer math in order
// to avoid floating point imprecision (plus it's probably faster).
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct Hline {
    int x1;
    int x2;
    int y;
};
struct Vline {
    int x;
    int y1;
    int y2;
};
struct Altar {
    int x1;
    int y1;
    int x2;
    int y2;
    int direction;  // 0 - E
                    // 1 - S
                    // 2 - W
                    // 3 - N
};
struct Angle {
    int x;
    int y;
};
// angles can be compared only if both x's are positive
bool operator<(Angle a1, Angle a2) {
    return a1.y * a2.x < a2.y * a1.x;
}
bool operator==(Angle a1, Angle a2) {
    return a1.y * a2.x == a2.y * a1.x;
}
struct Event {
    Angle a;
    bool type;  // true = begin, false = end
};
bool operator<(const Event& e1, const Event& e2) {
    return e1.a < e2.a || (e1.a == e2.a && e1.type < e2.type);
}
bool can_escape(Altar altar,
                const vector<Hline>& hlines,
                const vector<Vline>& vlines) {
    const int cx = (altar.x1 + altar.x2) / 2;
    const int cy = (altar.y1 + altar.y2) / 2;
    const Angle low = {altar.x2 - cx, -(altar.y2 - altar.y1)/4};
    const Angle high = {altar.x2 - cx, (altar.y2 - altar.y1)/4};
    vector<Event> events;
    for (auto hline : hlines) {
        if (hline.x2 <= altar.x2) continue;
        hline.x1 -= cx;
        hline.x2 -= cx;
        hline.y -= cy;
        const Angle a1 = {hline.x1, hline.y};
        const Angle a2 = {hline.x2, hline.y};
        if (hline.y > 0) {
            if (!(a2 < high)) continue;
            events.push_back(Event{a2, true});
            if (hline.x1 > 0 && a1 < high) {
                events.push_back(Event{a1, false});
            }
        } else if (hline.y < 0) {
            if (!(low < a2)) continue;
            events.push_back(Event{a2, false});
            if (hline.x1 > 0 && low < a1) {
                events.push_back(Event{a1, true});
            } else {
                events.push_back(Event{low, true});
            }
        } else {
            // This line takes up no angular space; you can always adjust your
            // angle infinitesimally unless there's something else there.
            continue;
        }
    }
    for (auto vline : vlines) {
        if (vline.x <= altar.x2) continue;
        vline.x -= cx;
        vline.y1 -= cy;
        vline.y2 -= cy;
        const Angle a1 = {vline.x, vline.y1};
        const Angle a2 = {vline.x, vline.y2};
        if (!(a1 < high && low < a2)) continue;
        if (low < a1) {
            events.push_back(Event{a1, true});
        } else {
            events.push_back(Event{low, true});
        }
        if (a2 < high) {
            events.push_back(Event{a2, false});
        }
    }
    sort(events.begin(), events.end());
    int cnt = 0; Angle last = low;
    for (const auto& event : events) {
        if (cnt == 0 && last < event.a) return true;
        last = event.a;
        if (event.type) ++cnt; else --cnt;
    }
    return cnt == 0 && last < high;
}

void do_testcase() {
    int n; cin >> n;
    vector<Hline> hlines;
    vector<Vline> vlines;
    vector<Altar> altars;
    vector<int> turns;
    for (int i = 0; i < n; i++) {
        Altar a; string direction;
        cin >> a.x1 >> a.y2 >> a.x2 >> a.y1 >> direction;
        a.x1 *= 4;
        a.y1 *= 4;
        a.x2 *= 4;
        a.y2 *= 4;
        if (direction[0] == 'E') {
            a.direction = 0;
            hlines.push_back(Hline{a.x1, a.x2, a.y1});
            hlines.push_back(Hline{a.x1, a.x2, a.y2});
            vlines.push_back(Vline{a.x1, a.y1, a.y2});
            vlines.push_back(Vline{a.x2, a.y1, a.y1 + (a.y2 - a.y1)/4});
            vlines.push_back(Vline{a.x2, a.y2 - (a.y2 - a.y1)/4, a.y2});
        } else if (direction[0] == 'N') {
            a.direction = 1;
            vlines.push_back(Vline{a.x1, a.y1, a.y2});
            vlines.push_back(Vline{a.x2, a.y1, a.y2});
            hlines.push_back(Hline{a.x1, a.x2, a.y2});
            hlines.push_back(Hline{a.x1, a.x1 + (a.x2 - a.x1)/4, a.y1});
            hlines.push_back(Hline{a.x2 - (a.x2 - a.x1)/4, a.x2, a.y1});
        } else if (direction[0] == 'W') {
            a.direction = 2;
            hlines.push_back(Hline{a.x1, a.x2, a.y1});
            hlines.push_back(Hline{a.x1, a.x2, a.y2});
            vlines.push_back(Vline{a.x2, a.y1, a.y2});
            vlines.push_back(Vline{a.x1, a.y1, a.y1 + (a.y2 - a.y1)/4});
            vlines.push_back(Vline{a.x1, a.y2 - (a.y2 - a.y1)/4, a.y2});
        } else {
            a.direction = 3;
            vlines.push_back(Vline{a.x1, a.y1, a.y2});
            vlines.push_back(Vline{a.x2, a.y1, a.y2});
            hlines.push_back(Hline{a.x1, a.x2, a.y1});
            hlines.push_back(Hline{a.x1, a.x1 + (a.x2 - a.x1)/4, a.y2});
            hlines.push_back(Hline{a.x2 - (a.x2 - a.x1)/4, a.x2, a.y2});
        }
        altars.push_back(a);
    }
    vector<int> escape;
    // To simplify the implementation, we'll first check whether any
    // east-facing altars can have evil spirits, then we'll rotate the entire
    // world by 90 degrees, repeat, and then repeat 2 more times.
    for (int direction = 0; direction < 4; direction++) {
        for (int i = 0; i < n; i++) {
            if (altars[i].direction != 0) continue;
            if (can_escape(altars[i], hlines, vlines)) {
                escape.push_back(i);
            }
        }
        // rotate all elements CCW 90 degrees
        vector<Hline> hlines2;
        vector<Vline> vlines2;
        for (const auto hline : hlines) {
            vlines2.push_back(Vline{-hline.y, hline.x1, hline.x2});
        }
        for (const auto vline : vlines) {
            hlines2.push_back(Hline{-vline.y2, -vline.y1, vline.x});
        }
        hlines = move(hlines2);
        vlines = move(vlines2);
        for (auto& altar : altars) {
            const Altar new_altar = {-altar.y2, altar.x1, -altar.y1, altar.x2,
                                     (altar.direction + 1) % 4};
            altar = new_altar;
        }
    }
    if (escape.empty()) {
        cout << "NONE\n";
    } else {
        sort(escape.begin(), escape.end());
        for (const auto i : escape) {
            cout << i + 1 << '\n';
        }
    }
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
