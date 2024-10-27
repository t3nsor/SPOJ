// 2024-10-27
// At least some lines of input end with CRLF, so watch out for that. Other than
// that, there isn't anything tricky about the input format.
//
// I'm not sure if you can get AC from brute force; on the DMOJ version of this
// problem, the answer is always less than 5 million. The time limit on SPOJ is
// more strict so it was probably designed to prevent brute force.
//
// The basic idea is simple: a program might potentially spend a lot of time
// repetitively executing a cycle of instructions. We want to either detect such
// cycles as infinite or quickly skip forward to the part where the cycle is
// broken. Every time the cycle is repeated, it'll do the same thing unless one
// of the branch instructions that was previously taken is no longer taken or
// vice versa. We can calculate the number of cycles required for that to happen
// by computing how much each variable is incremented by during one complete
// cycle, and comparing it with the difference between the value that the
// variable would need to have to change the disposition of each branch
// instruction in the cycle and the value that the variable has the first time
// the branch instruction is executed. This tells us how many times we have to
// simulate the entire cycle until something changes. Note that the number of
// times we have to go through this whole procedure is at most twice the number
// of branch instructions in the program, since a branch instruction can only
// transition from not-taken to taken once and from taken to not-taken once.
#include <algorithm>
#include <ctype.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
enum Opcode {
    ADD,
    NOP,
    GOTO,
    COND,
    END,
};
struct Instruction {
    Opcode opcode;
    int var;    // variable to increment or test (0 = A, etc)
    int value;  // value to add or test against
    int label;  // instruction to go to next (0 based index)
};
struct State {
    int pc;          // current instruction
    long long time;  // # of instructions previously executed
    int value[26];   // values of variables
};
bool advance(State& state, const vector<Instruction>& prog) {
    ++state.time;
    const Instruction& inst = prog[state.pc];
    if (inst.opcode == END) return false;
    if (inst.opcode == NOP) {
        ++state.pc;
    } else if (inst.opcode == ADD) {
        ++state.pc;
        state.value[inst.var] += inst.value;
    } else if (inst.opcode == GOTO) {
        state.pc = inst.label;
    } else {
        if (state.value[inst.var] == inst.value) state.pc = inst.label;
        else ++state.pc;
    }
    return true;
}
long long analyze(const vector<Instruction>& prog) {
    const int N = prog.size();
    State state{};
    for (;;) {
        // advance until an instruction repeats
        State oldstate = state;
        vector<char> vis(N);
        while (!vis[state.pc]) {
            vis[state.pc] = 1;
            if (!advance(state, prog)) return state.time;
        }
        // rewind to the first instruction in the cycle
        while (oldstate.pc != state.pc) advance(oldstate, prog);
        state = oldstate;
        // make a list of increments and conditional jumps per cycle
        int incr[26]{};
        vector<pair<Instruction, int>> conds;
        int cycle_size = 0;
        do {
            if (prog[state.pc].opcode == ADD) {
                incr[prog[state.pc].var] += prog[state.pc].value;
            } else if (prog[state.pc].opcode == COND) {
                conds.emplace_back(prog[state.pc],
                                   state.value[prog[state.pc].var]);
            }
            advance(state, prog);
            ++cycle_size;
        } while (state.pc != oldstate.pc);
        state = oldstate;
        // find the first conditional jump whose disposition will change
        long long min_cycles = 1LL << 32;
        for (int i = 0; i < conds.size(); i++) {
            if (incr[conds[i].first.var] == 0 ||
                conds[i].second > conds[i].first.value) continue;
            if (conds[i].second == conds[i].first.value) {
                // the next time this instruction is reached, the jump WILL NOT
                // be taken
                min_cycles = 1;
                break;
            }
            if ((conds[i].first.value - conds[i].second) %
                incr[conds[i].first.var] == 0) {
                // after a number of cycles, the jump WILL be taken
                const int cycles = (conds[i].first.value - conds[i].second) /
                                   incr[conds[i].first.var];
                min_cycles = min(min_cycles, (long long)cycles);
            }
        }
        if (min_cycles == (1LL << 32)) return -1;
        state.time += min_cycles * cycle_size;
        for (int i = 0; i < 26; i++) {
            state.value[i] += incr[i] * min_cycles;
        }
        // and repeat
    }
}
int main() {
    vector<pair<int, Instruction>> input;
    for (;;) {
        int label; cin >> label;
        if (cin.eof()) break;
        Instruction inst {};
        string line; getline(cin, line);
        // according to Jacob there is some issue with the format of some lines,
        // so let's assume there are extra spaces
        line.erase(0, 1);
        if (line.back() == '\r') line.pop_back();
        if (line == "END") {
            inst.opcode = END;
        } else if (line.back() == '?') {
            inst.opcode = NOP;
        } else if (line[1] == '+') {
            inst.opcode = ADD;
            inst.var = line[0] - 'A';
            line.erase(0, 2);
            inst.value = stoi(line);
        } else if (line[0] == 'G') {
            inst.opcode = GOTO;
            line.erase(0, 3);
            inst.label = stoi(line);
        } else {
            inst.opcode = COND;
            inst.var = line[3] - 'A';
            line.erase(0, 5);
            inst.value = 0;
            while (isdigit(line[0])) {
                inst.value = 10 * inst.value + line[0] - '0';
                line.erase(0, 1);
            }
            line.erase(0, 4);
            inst.label = stoi(line);
        }
        input.emplace_back(label, inst);
    }
    sort(input.begin(), input.end(),
         [](auto x, auto y) {
             return x.first < y.first;
         });

    vector<Instruction> prog(input.size()); 
    for (int i = 0; i < input.size(); i++) {
        prog[i] = input[i].second;
        if (input[i].second.opcode == GOTO || input[i].second.opcode == COND) {
            int j = 0;
            while (input[j].first != input[i].second.label) j++;
            prog[i].label = j;
        }
    }
    cout << analyze(prog) << '\n';
}
