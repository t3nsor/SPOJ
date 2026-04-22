// 2026-04-21
// We use the following memory layout:
// 9998: zero register (Z), also used as scratch space but must be restored
//       after any use
// 9997: scratch register (Y) used only by division, cannot be an argument
// 9996: scratch register (X), cannot be an argument
// x ... 9995: immediate operands, copies of M0 ... M8, and results of
//             operators.
//
// Whenever the values of registers are used as operands, they are copied to a
// previously unused memory location first.  This allows the implementations of
// the operations themselves to be destructive.  Immediate operands are also
// stored into memory so that the implementation of each operation can assume
// that the operands and result are always memory locations.  Some operations
// are given a previously unused memory location to store their result into;
// others overwrite the first operand.
//
// The implementations of multiplication and division are naive: repeated
// addition and repeated subtraction, respectively.  In the multiplication case,
// we need to make the first operand non-negative so that it can count down to
// zero.  In the division case, we need to make the dividend and divisor have
// the same sign (or the dividend equal to zero) so that repeated subtraction
// goes in the right direction.  My implementation is a bit more complicated
// because I wanted to be able to modify it easily to do truncating division if
// I got WA, in case that is what the judge wants.  (It turns out that I got AC
// assuming that the dividend is always an integer multiple of the divisor.)  So
// my algorithm always makes both operands positive and maintains an extra sign
// bit to give the result the correct sign.  Because of this, the division
// routine is too long to be inlined 99 times (as it would be in the worst-case
// scenario), so the code for division is instead emitted only once and we have
// to overwrite the address for it to return to every time we need to use it.
//
// This solution includes "debugging" features, including the possibility of
// defining breakpoints in the emitted code.  Calling `breakpoint` before a
// call to `subleq` (or any other function that ultimately emits `subleq`)
// instructions tells the simulator to print a diagnostic when it reaches the
// immediately-following instruction.  When running this program in a debugger,
// you can break on the line that prints the diagnostic if you want to examine
// the contents of memory at that point in time.

#include <algorithm>
#include <ctype.h>
#include <iostream>
#ifdef DBG
 #include <map>
#endif
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int pc;
const int Z = 9998;
const int Y = 9997;
const int X = 9996;
const int divisor = 9995;
const int dividend = 9994;
const int quotient = 9993;
const int var_start = 9992;
int div_start;
int div_ret;
int entrypoint;  // after end of division routine

int memory[9999];

#ifdef DBG
map<int, string> breakpoints;

void breakpoint(string label) {
    breakpoints[pc] = label;
}
#endif

void subleq(int a, int b, int c) {
    memory[pc++] = a;
    memory[pc++] = b;
    memory[pc++] = c;
}

void subleq(int a, int b) {
    memory[pc++] = a;
    memory[pc++] = b;
    memory[pc] = pc + 1;
    ++pc;
}

void zero(int loc) {
    subleq(loc, loc);
}

void store_positive(int loc, int val) {
    zero(loc);
    subleq(-val, loc);
}

// `src` and `dest` must be distinct memory locations
void copy(int src, int dest) {
    subleq(X, X);
    subleq(src, X);
    subleq(dest, dest);
    subleq(X, dest);
}

// `accum` and `loc` must be distinct memory locations
void add(int accum, int loc) {
    subleq(X, X);
    subleq(loc, X);
    subleq(X, accum);
}

// `accum` and `loc` must be distinct memory locations
void sub(int accum, int loc) {
    subleq(loc, accum);
}

void flip(int loc) {
    copy(loc, Z);
    zero(loc);
    sub(loc, Z);
    zero(Z);
}

// all operands must be distinct memory locations and cannot be X, Y, or Z
void mul(int a, int b, int prod) {
    zero(prod);
    subleq(a, Z);  int& needs_l1 = memory[pc - 1];
    zero(Z);
    flip(a);
    flip(b);
    needs_l1 = pc;
    zero(Z);
    int l2 = pc;
    subleq(Z, a);  int& needs_l3 = memory[pc - 1];
    store_positive(Z, 1);
    sub(a, Z);
    zero(Z);
    add(prod, b);
    subleq(Z, Z, l2);
    needs_l3 = pc;
}

void define_div() {
    div_start = pc;
    const int a = dividend;
    const int b = divisor;
    const int c = quotient;
    zero(c);
    store_positive(Z, 1);
    zero(Y);
    subleq(Z, Y);  // set Y to -1 if *no* flips are required ...
    subleq(a, Z);  int& needs_l1 = memory[pc - 1];
    zero(Z);
    flip(a);
    flip(Y);
    needs_l1 = pc;
    zero(Z);
    subleq(b, Z);  int& needs_l2 = memory[pc - 1];
    zero(Z);
    flip(b);
    flip(Y);
    needs_l2 = pc;
    zero(Z);
    int l3 = pc;
    subleq(Z, a);  int& needs_l4 = memory[pc - 1];
    subleq(b, a);
    store_positive(Z, 1);
    subleq(Z, c);
    subleq(Z, Z, l3);
    needs_l4 = pc;
    // The value of c produced is the negative of the quotient in case the
    // dividend and divisor were both originally positive.  This is why Y was
    // set to -1 initially: we need to flip the sign in that case.
    subleq(Y, Z);  int& needs_l5 = memory[pc - 1];
    flip(c);
    needs_l5 = pc;
    subleq(Z, Z);
    div_ret = pc - 1;
}

// all operands must be distinct memory locations and cannot be X, Y, or Z
void div(int a, int b, int quot) {
    copy(a, dividend);
    copy(b, divisor);
    int return_address = pc + 9;
    store_positive(div_ret, return_address);  // after the next instruction
    subleq(Z, Z, div_start);
    if (return_address != pc) throw;
    copy(quotient, quot);
}

void do_testcase(string line) {
    string token;
    istringstream iss(line);
    pc = entrypoint;
    vector<int> stk;
    int var = var_start;
    while (iss >> token) {
        if (token[0] == 'M') {
            // always copy the value to a variable to avoid destroying its value
            int src = token[1] - '0';
            copy(src, var);
            stk.push_back(var--);
        } else if (isdigit(token[0])) {
            const int val = stoi(token);
            if (val == 0) zero(var); else store_positive(var, val);
            stk.push_back(var--);
        } else if (token == "+") {
            add(stk[stk.size() - 2], stk[stk.size() - 1]);
            stk.pop_back();
        } else if (token == "-") {
            sub(stk[stk.size() - 2], stk[stk.size() - 1]);
            stk.pop_back();
        } else if (token == "*") {
            mul(stk[stk.size() - 2], stk[stk.size() - 1], var);
            stk.pop_back();
            stk.back() = var--;
        } else if (token == "/") {
            div(stk[stk.size() - 2], stk[stk.size() - 1], var);
            stk.pop_back();
            stk.back() = var--;
        } else {
            throw;
        }
    }
    copy(stk[0], 0);
    subleq(Z, Z, 10000);
    cout << (pc - 9) / 3 << '\n';
    for (int i = 9; i < pc; i += 3) {
#ifdef DBG
        cout << i << ": ";
#endif
        cout << memory[i] << ' ' << memory[i + 1] << ' ' << memory[i + 2]
             << '\n';
    }
}

#ifdef DBG
void test(string line, int* M) {
    // compute expected answer
    string token;
    istringstream iss(line);
    vector<int> stk;
    while (iss >> token) {
        if (token[0] == 'M') {
            stk.push_back(M[token[1] - '0']);
        } else if (isdigit(token[0])) {
            stk.push_back(stoi(token));
        } else if (token == "+") {
            stk[stk.size() - 2] += stk[stk.size() - 1];
            stk.pop_back();
        } else if (token == "-") {
            stk[stk.size() - 2] -= stk[stk.size() - 1];
            stk.pop_back();
        } else if (token == "*") {
            stk[stk.size() - 2] *= stk[stk.size() - 1];
            stk.pop_back();
        } else if (token == "/") {
            stk[stk.size() - 2] /= stk[stk.size() - 1];
            stk.pop_back();
        } else throw;
    }
    int expected = stk[0];
    // simulate actual
    for (int i = 0; i <= 8; i++) memory[i] = M[i];
    int pc = 9;
    while (pc < 9997) {
        int A = memory[pc]; int B = memory[pc + 1]; int C = memory[pc + 2];
        if (breakpoints.count(pc)) {
            cerr << "Breakpoint " << breakpoints[pc] << ": " << A << ' '
                 << B << ' ' << C << '\n';
        }
        if (A >= 0) {
            memory[B] = memory[B] - memory[A];
        } else {
            memory[B] = memory[B] - A;
        }
        if (memory[B] > 0) pc = pc + 3; else pc = C;
    }
    int actual = memory[0];
    if (actual == expected) {
        cout << "test passes, result is " << expected;
    } else {
        cout << "FAIL: expected " << expected << ", got " << actual;
    }
    cout << '\n';
}
#endif

int main() {
    ios::sync_with_stdio(false);
    pc = 9;
    subleq(Z, Z);  int& needs_entrypoint = memory[pc - 1];
    define_div();
    entrypoint = pc;
    needs_entrypoint = entrypoint;
    bool first = true;
    for (;;) {
        if (cin.eof()) break;
        string line; getline(cin, line);
        // defend against blank lines (which can themselves have bs such as
        // carriage returns)
        if (all_of(line.begin(), line.end(), [](int c) { return c <= 32; })) {
            continue;
        }
        // sample output shows a blank line between cases
        if (!first) cout << '\n';
        first = false;
        do_testcase(line);
#ifdef DBG
        cout << "enter test data: ";
        string testdata; getline(cin, testdata);
        int i = 0, val;
        int M[9];
        istringstream iss(testdata);
        while (i <= 8 && iss >> val) {
            M[i++] = val;
        }
        test(line, M);
#endif
    }
}
