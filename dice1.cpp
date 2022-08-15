// 2022-08-15
// For short distances, Dijkstra's suffices, although we have to find a way to
// correctly handle all 24 possible orientations of the die and how they
// transform into each other upon rotation. For long distances, we guess that
// when we go out a sufficiently large number of columns, the distances become
// periodic (in the sense that the distance to some square+orientation equals
// the distance to some closer square in the same orientation plus some fixed
// delta). A priori, it is difficult to determine possible values of the period,
// so instead we just search for a period every time we "complete" a column
// (i.e., reach all reachable square+orientation combinations in that column
// during Dijkstra's). Assuming that the periodicity is found within a small
// number of columns, this should be fast enough. If there is a test case where
// the periodicity is not reached for tens of thousands of columns, then this
// solution won't be fast enough, but I don't think such test cases can exist
// given the constraints in the problem. It would be more interesting to
// consider what would happen if the face costs could be irrational numbers.
#include <algorithm>
#include <array>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

// orientation
struct O {
    // P represents a position that a face can occupy
    enum P {
        TOP = 0,
        FRONT = 1,
        LEFT = 2,
        RIGHT = 3,
        BACK = 4,
        BOTTOM = 5,
        INVALID = 6
    };
    P pos0 = TOP;    // position of face that was originally on the top
    P pos1 = FRONT;  // position of face that was originally on the front
    int serialize() const {
        static int a[6][6] = {
            {-1,  0,  1,  2,  3, -1},
            { 4, -1,  5,  6, -1,  7},
            { 8,  9, -1, -1, 10, 11},
            {12, 13, -1, -1, 14, 15},
            {16, -1, 17, 18, -1, 19},
            {-1, 20, 21, 22, 23, -1}
        };
        return a[pos0][pos1];
    }
    // static O deserialize(int i) {
    //     static int a[24][2] = {
    //         {0, 1}, {0, 2}, {0, 3}, {0, 4},
    //         {1, 0}, {1, 2}, {1, 3}, {1, 5},
    //         {2, 0}, {2, 1}, {2, 4}, {2, 5},
    //         {3, 0}, {3, 1}, {3, 4}, {3, 5},
    //         {4, 0}, {4, 2}, {4, 3}, {4, 5},
    //         {5, 1}, {5, 2}, {5, 3}, {5, 4}
    //     };
    //     return {P(a[i][0]), P(a[i][1])};
    // }
    static P rotf(P p) {
        switch (p) {
            case TOP: return FRONT;
            case FRONT: return BOTTOM;
            case BOTTOM: return BACK;
            case BACK: return TOP;
            default: return p;
        }
    }
    static P rotb(P p) {
        switch (p) {
            case TOP: return BACK;
            case BACK: return BOTTOM;
            case BOTTOM: return FRONT;
            case FRONT: return TOP;
            default: return p;
        }
    }
    static P rotl(P p) {
        switch (p) {
            case TOP: return LEFT;
            case LEFT: return BOTTOM;
            case BOTTOM: return RIGHT;
            case RIGHT: return TOP;
            default: return p;
        }
    }
    static P rotr(P p) {
        switch (p) {
            case TOP: return RIGHT;
            case RIGHT: return BOTTOM;
            case BOTTOM: return LEFT;
            case LEFT: return TOP;
            default: return p;
        }
    }
    void rotf() {
        pos0 = rotf(pos0);
        pos1 = rotf(pos1);
    }
    void rotb() {
        pos0 = rotb(pos0);
        pos1 = rotb(pos1);
    }
    void rotl() {
        pos0 = rotl(pos0);
        pos1 = rotl(pos1);
    }
    void rotr() {
        pos0 = rotr(pos0);
        pos1 = rotr(pos1);
    }
    int top() const {
        // who is on top now
        static int a[6][6] = {
            // 0 is on the top
            {-1,  0,  0,  0,  0, -1},
            // 0 is on the front and 1 is on ...
            { 1, -1,  2,  3, -1,  4},
            // 0 is on the left and 1 is on ...
            { 1,  3, -1, -1,  2,  4},
            // 0 is on the right and 1 is on ...
            { 1,  2, -1, -1,  3,  4},
            // 0 is on the back and 1 is on ...
            { 1, -1,  3,  2, -1,  4},
            // 0 is on the bottom
            {-1,  5,  5,  5,  5, -1}
        };
        return a[pos0][pos1];
    }
};

void do_testcase() {
    int l[6];
    cin >> l[0] >> l[1] >> l[2] >> l[3] >> l[4] >> l[5];
    int orig_x1, y1, orig_x2, y2;
    cin >> orig_x1 >> y1 >> orig_x2 >> y2;
    --y1; --y2;
    int x = orig_x2 - orig_x1;  // only the difference is relevant
    if (x < 0) {
        // reflect so that the destination is to the right
        x = -x;
        swap(l[2], l[3]);
    }
    struct Column {
        array<int, 96> data;
        int need;
        Column() {
            fill(data.begin(), data.end(), 1000000000);
            // Each movement of the cube induces a cyclic permutation of the
            // cube's four body diagonals, and a 4-cycle is an odd permutation.
            // The parity of the cube's coordinates also flips. It follows that
            // the sum of the two parities is left unchanged, and the other
            // half of the configurations are inaccessible.
            need = 48;
        }
    };
    vector<Column> dist;
    struct Node {
        int dist;
        int x;
        int y;
        O o;
        bool operator<(Node other) const {
            return dist > other.dist;
        }
    };
    priority_queue<Node> Q;
    Q.push(Node{0, 0, y1, O{}});
    int x_left, x_right, need = 0;
    int period = -1, ddist;
    for (;;) {
        Node node = Q.top(); Q.pop();
        if (node.x == x && node.y == y2) {
            cout << node.dist << '\n';
            return;
        }
        if (node.x + 4 >= (int)dist.size()) dist.resize(node.x + 5);
        auto& c = dist[node.x + 4];
        int i = 24 * node.y + node.o.serialize();
        if (c.data[i] < 1000000000) continue;
        c.data[i] = node.dist;
        if (0 == --c.need) {
            if (period == -1) {
                // check for periodicity
                for (x_left = 0; ; ++x_left) {
                    if (x_left + 4 >= (int)dist.size()) break;
                    if (x_left == node.x) continue;
                    auto& c2 = dist[x_left + 4];
                    if (0 != c2.need) continue;
                    ddist = 1000000000;
                    bool success = true;
                    for (int i = 0; i < 96; i++) {
                        if (c.data[i] == 1000000000) continue;
                        if (ddist == 1000000000) {
                            ddist = c.data[i] - c2.data[i];
                        }
                        if (c.data[i] - c2.data[i] != ddist) {
                            success = false;
                            break;
                        }
                    }
                    if (success) {
                        period = abs(node.x - x_left);
                        x_right = max(x_left, node.x);
                        x_left = min(x_left, node.x);
                        for (int i = x_left; i <= x_right; i++) {
                            if (dist[i + 4].need > 0) ++need;
                        }
                        break;
                    }
                }
            }
            else if (node.x >= x_left && node.x <= x_right) {
                --need;
            }
            if (period > 0 && 0 == need) {
                // the period is filled in
                const long long periods = (x - x_left) / period;
                const int offset = (x - x_left) % period;
                const int bx = x_left + offset;
                int bdist = 1000000000;
                for (int i = 0; i < 24; i++) {
                    bdist = min(bdist, dist[bx + 4].data[i + 24 * y2]);
                }
                const long long result = bdist + periods * ddist;
                cout << result << '\n';
                cerr << "PERIOD: " << period << '\n';
                cerr << "LEFT: " << x_left << '\n';
                cerr << "RIGHT: " << x_right << '\n';
                return;
            }
        }
        // add neighbours to priority queue
        if (node.y > 0) {
            O o2 = node.o;
            o2.rotf();
            const int cost = l[o2.top()];
            Q.push(Node{node.dist + cost, node.x, node.y - 1, o2});
        }
        if (node.y < 3) {
            O o2 = node.o;
            o2.rotb();
            const int cost = l[o2.top()];
            Q.push(Node{node.dist + cost, node.x, node.y + 1, o2});
        }
        if (node.x > -4) {
            O o2 = node.o;
            o2.rotl();
            const int cost = l[o2.top()];
            Q.push(Node{node.dist + cost, node.x - 1, node.y, o2});
        }
        {
            O o2 = node.o;
            o2.rotr();
            const int cost = l[o2.top()];
            Q.push(Node{node.dist + cost, node.x + 1, node.y, o2});
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
