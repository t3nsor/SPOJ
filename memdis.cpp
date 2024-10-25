// 2024-10-24
// Because there are only up to 10,000 programs per test case, a sqrt
// decomposition method looks attractive. But we won't be able to divide up the
// programs into groups of 100 perfectly because we don't know where they're
// going to wind up in memory ahead of time. So we just have to split a group
// every time it gets too big.
//
// (Because the SPOJ judge is fast, I wonder if a pure quadratic time approach
// would have worked. Wouldn't be much fun to code, though.)
//
// Each group contains a sorted list of memory ranges that are currently in use
// and keeps track of the size of the largest gap. To add a new program, we
// look for a gap that is big enough to accommodate it, and if that gap is known
// to be inside a group (because the group's max gap is at least as large as the
// program's memory requirement) we iterate over the group to find the leftmost
// gap that is big enough.
//
// The only tricky aspect is how to handle events that occur simultaneously.
// Based on the sample data, if a program finishes running at time T, then a
// program that has been queued up takes priority over a new program whose X
// value is T. Also, when multiple programs finish running at the same time T,
// no program can be removed from the queue and scheduled at time T until all
// programs finishing at time T have freed their memory. The problem statement
// ought to be more clear about these points. It was translated from Chinese, so
// maybe something was lost in the process.
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
struct block {
    int begin;
    int end;
};
struct Blocks {
    vector<block> blocks;
    int max_gap_size = -1;
    int count() const { return blocks.size(); }
    int begin() const { return blocks.front().begin; }
    int end() const { return blocks.back().end; }
    void recalc_gap() {
        max_gap_size = -1;
        for (int i = 1; i < blocks.size(); i++) {
            max_gap_size = max(max_gap_size,
                               blocks[i].begin - blocks[i - 1].end);
        }
    }
    void insert(block b) {
        int pos = 0;
        while (pos < blocks.size() && blocks[pos].begin < b.begin) {
            ++pos;
        }
        blocks.insert(blocks.begin() + pos, b);
        recalc_gap();
    }
    int insert(int size) {
        int pos = 1;
        while (blocks[pos].begin - blocks[pos - 1].end < size) ++pos;
        block b = {blocks[pos - 1].end, blocks[pos - 1].end + size};
        blocks.insert(blocks.begin() + pos, b);
        recalc_gap();
        return b.begin;
    }
    void remove(int start) {
        int pos = 0;
        while (blocks[pos].begin != start) ++pos;
        blocks.erase(blocks.begin() + pos);
        recalc_gap();
    }
    Blocks split() {
        Blocks secondhalf;
        secondhalf.blocks.assign(blocks.begin() + blocks.size() / 2,
                                 blocks.end());
        secondhalf.recalc_gap();
        blocks.erase(blocks.begin() + blocks.size() / 2, blocks.end());
        recalc_gap();
        return secondhalf;
    }
};
struct Memory {
    const int N;
    vector<Blocks> data;
    Memory(int N): N(N) {}
    int insert(int size) {
        if (data.empty()) {
            data.emplace_back();
            data.back().insert(block{0, size});
            return 0;
        }
        if (size <= data[0].begin()) {
            data[0].insert(block{0, size});
            if (data[0].count() > 140) {
                Blocks secondhalf = data[0].split();
                data.insert(data.begin() + 1, std::move(secondhalf));
            }
            return 0;
        }
        for (int i = 0; i < data.size(); i++) {
            int start = -1;
            if (data[i].count() >= 2) {
                if (data[i].max_gap_size >= size) {
                    start = data[i].insert(size);
                }
            }
            if (start < 0) {
                const int limit = (i + 1 < data.size())
                                  ? data[i + 1].begin()
                                  : N;
                if (limit - data[i].end() >= size) {
                    start = data[i].end();
                    data[i].insert({start, start + size});
                }
            }
            if (start >= 0) {
                if (data[i].count() > 140) {
                    Blocks secondhalf = data[i].split();
                    data.insert(data.begin() + i + 1, std::move(secondhalf));
                }
                return start;
            }
        }
        return -1;
    }
    void erase(int start) {
        for (int i = 0; i < data.size(); i++) {
            if (data[i].end() > start) {
                data[i].remove(start);
                if (data[i].count() == 0) {
                    data.erase(data.begin() + i);
                }
                return;
            }
        }
    }
};
struct Event {
    int time;
    int type;  // 0 to remove, 1 to add
    int data;  // ID if type == 1; start address if type == 0
    friend bool operator<(const Event& e1, const Event& e2) {
        return e1.time > e2.time ||
               (e1.time == e2.time && e1.type > e2.type);
    }
};
void do_testcase() {
    int N; cin >> N;
    Memory memory(N);
    vector<int> size, duration;
    priority_queue<Event> H;
    int nprog = 0;
    for (;;nprog++) {
        int X, M, P; cin >> X >> M >> P;
        if (X == 0 && M == 0 && P == 0) break;
        size.push_back(M);
        duration.push_back(P);
        H.push(Event{X, 1, nprog});
    }
    int tend = 0;
    int numScheduled = 0;
    int numQueued = 0;
    queue<int> Q;  // programs waiting to be run
    while (numScheduled < nprog) {
        Event e = H.top();
        H.pop();
        if (e.type == 1) {
            const int start = memory.insert(size[e.data]);
            if (start >= 0) {
                H.push(Event{e.time + duration[e.data], 0, start});
                tend = max(tend, e.time + duration[e.data]);
                ++numScheduled;
            } else {
                Q.push(e.data);
                ++numQueued;
            }
        } else {
            memory.erase(e.data);
            // peek at the next thing on the heap; don't start popping anything
            // off the queue until we've finished all removals for this time
            if (!H.empty() && H.top().time == e.time && H.top().type == 0) {
                continue;
            }
            while (!Q.empty()) {
                const int next = Q.front();
                const int start = memory.insert(size[next]);
                if (start == -1) break;
                Q.pop();
                H.push(Event{e.time + duration[next], 0, start});
                tend = max(tend, e.time + duration[next]);
                ++numScheduled;
            }
        }
    }
    cout << tend << '\n' << numQueued << '\n';
}
int main() {
    for (int i = 0; i < 10; i++) do_testcase();
}
