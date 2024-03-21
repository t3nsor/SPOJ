// 2024-03-21
// Greedy: sort the contracts by reverse order of deadline and to decide which
// job to work on at each time, start from the latest deadline and go backward.
// We can't do any work after the latest deadline. Before the latest deadline
// but after all other deadlines, we can only work on the job with that
// deadline. As we reach a deadline, we push the corresponding job onto a
// priority queue. We always work on the job that costs the most to expedite
// (found using the priority queue) and pop it off if it's done. When we reach
// time 0, we pay the programmer the amount needed so that all remaining jobs
// only use up the amount of time that we've already allocated to them.
#include <algorithm>
#include <queue>
#include <stdio.h>
#include <vector>
using namespace std;
struct Job {
    int id;
    int duration;
    int invrate;
    long long deadline;
};
struct DeadlineGreater {
    bool operator()(const Job& j1, const Job& j2) const {
        return j1.deadline > j2.deadline;
    }
};
struct RateLess {
    bool operator()(const Job& j1, const Job& j2) const {
        return j1.invrate > j2.invrate;
    }
};

void do_testcase() {
    int N; scanf("%d", &N);
    vector<Job> jobs(N + 1);
    vector<int> remaining(N + 1);
    for (int i = 0; i < N; i++) {
        jobs[i].id = i;
        scanf("%d %d %lld",
              &jobs[i].invrate, &jobs[i].duration, &jobs[i].deadline);
        remaining[i] = jobs[i].duration;
    }
    // add a fake job with deadline 0 to simplify the algorithm
    jobs[N].id = N;
    jobs[N].deadline = 0;
    remaining[N] = jobs[N].duration = 0;
    jobs[N].invrate = 1;
    sort(jobs.begin(), jobs.end(), DeadlineGreater());
    priority_queue<Job, vector<Job>, RateLess> Q;
    long long currentTime;
    for (int i = 0; i < jobs.size(); i++) {
        while (!Q.empty()) {
            const auto currentJob = Q.top();
            if (remaining[currentJob.id] <= currentTime - jobs[i].deadline) {
                // we can finish the job
                currentTime -= remaining[currentJob.id];
                Q.pop();
            } else {
                // do as much as we can now
                remaining[currentJob.id] -= currentTime - jobs[i].deadline;
                break;
            }
        }
        currentTime = jobs[i].deadline;
        Q.push(jobs[i]);
    }
    // the current time is now 0, pay off all the remaining jobs
    long double result = 0.0;
    while (!Q.empty()) {
        const auto job = Q.top();
        Q.pop();
        result += remaining[job.id] / (long double)job.invrate;
    }
    printf("%.2Lf\n", result);
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
