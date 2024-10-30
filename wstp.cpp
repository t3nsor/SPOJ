/*
wstp: Transpiler from human-readable text to whitespace

SPOJ uses version 0.3 of wspace (the original Haskell implementation).
The documentation is not great. Based on the Haskell source, this is what the
commands mean (using the notation below):

=== Stack manipulation ===
  push n    Push n onto stack
  copy n    Copy the nth element from the top (0-based).
            (The copied value is pushed onto the stack.)
  slide n   Remove elements at indices 1 ... n from the stack (0-based).
  dup       Duplicate top element of stack
  swap      Swap top 2 elements of stack
  pop       Discard top element of stack

=== Arithmetic ===
Let y be the top value and x the next to top value.
Both values are popped, and the result is then pushed:
  add       x + y
  sub       x - y
  mul       x * y
  div       div x y as in Haskell (rounds to minus infinity)
  mod       mod x y as in Haskell (rounds like div)

=== Heap manipulation ===
i must be nonnegative.
  store     Let x be the top value and i second from the top.
            Store x into heap memory location i, and pop both values.
  load      Let i be the top value. Pop this off, load the value at heap
            memory location i, and push it onto the stack.

=== I/O ===
  writec    Pop stack and write the value as a character to stdout
  writen    Pop stack and write the value as an integer to stdout. Newline is
            not written.
  readc     Let i be the top value. Pop this off, read a character from
            stdin, and store its value at heap memory location i
  readn     Let i be the top value. Pop this off. Read characters until the end
            of line; the resulting string is interpreted as an integer (there
            will be an error if there are any trailing non-digits). Store the
            integer value at heap memory location i.

=== Flow control ===
  label L   Mark label here (should be unique, but this isn't enforced)
  call L    Call subroutine at label L
  jump L    Unconditional jump to label L
  jzero L   Pop stack and jump to label L if the value is zero
  jneg  L   Pop stack and jump to label L if the value is negative
  ret       Return to the instruction after the most recent call instruction
  exit      End program

=== Numbers and labels ===
n denotes an integer. In whitespace, this is encoded as a sign bit (space =
positive, tab = negative) followed by zero or more value bits (most
significant bit first) where space = 0 and tab = 1, and terminated by a
newline. When no value bits are present, the value is zero.

L denotes a label. wstp requires labels to be nonempty alphanumeric strings.
In whitespace, a label is just a sequence of spaces and tabs, terminated by a
newline. I haven't tested whether said sequence is allowed to be empty.

=== Comments ===
In wstp input, a comment is denoted by the word "#" followed by zero or more
non-newline characters terminated by a newline. In whitespace, any character
other than space, tab, or newline is a comment. But wstp simply discards
comments from its input and doesn't produce any non-whitespace characters.
*/

#include <algorithm>
#include <cctype>
#include <iostream>
#include <optional>
#include <string>
using namespace std;
string num_to_ws(int value) {
    bool minus = false;
    string result;
    if (value < 0) {
        value *= -1;
        minus = true;
    }
    while (value) {
        if (value % 2 == 0) {
            result.push_back(' ');
        } else {
            result.push_back('\t');
        }
        value /= 2;
    }
    reverse(result.begin(), result.end());
    if (minus) {
        result = '\t' + result;
    } else {
        result = ' ' + result;
    }
    result.push_back('\n');
    return result;
}
string str_to_ws(string s) {
    string result;
    for (unsigned char c : s) {
        if (!isalnum(c)) throw;
        for (int i = 0; i < 8; i++) {
            result.push_back(c % 2 == 0 ? ' ' : '\t');
            c /= 2;
        }
    }
    result.push_back('\n');
    return result;
}
optional<string> get_word() {
    optional<string> result;
    string word;
retry:
    if (cin >> word) {
        if (word == "#") {
            // comment
            while (cin.get() != '\n') {}
            goto retry;
        } else {
            result.emplace(move(word));
        }
    }
    return result;
}
int main() {
    while (const auto cmd = get_word()) {
        if (*cmd == "push" || *cmd == "copy" || *cmd == "slide") {
            const auto num = get_word();
            if (!num) {
                cerr << "expected numeric operand\n";
                return 1;
            }
            if (*cmd == "push") {
                cout << "  " << num_to_ws(stoi(*num));
            } else if (*cmd == "copy") {
                cout << " \t " << num_to_ws(stoi(*num));
            } else {
                cout << " \t\n" << num_to_ws(stoi(*num));
            }
        } else if (*cmd == "dup") {
            cout << " \n ";
        } else if (*cmd == "swap") {
            cout << " \n\t";
        } else if (*cmd == "pop") {
            cout << " \n\n";
        } else if (*cmd == "add") {
            cout << "\t   ";
        } else if (*cmd == "sub") {
            cout << "\t  \t";
        } else if (*cmd == "mul") {
            cout << "\t  \n";
        } else if (*cmd == "div") {
            cout << "\t \t ";
        } else if (*cmd == "mod") {
            cout << "\t \t\t";
        } else if (*cmd == "store") {
            cout << "\t\t ";
        } else if (*cmd == "load") {
            cout << "\t\t\t";
        } else if (*cmd == "writec") {
            cout << "\t\n  ";
        } else if (*cmd == "writen") {
            cout << "\t\n \t";
        } else if (*cmd == "readc") {
            cout << "\t\n\t ";
        } else if (*cmd == "readn") {
            cout << "\t\n\t\t";
        } else if (*cmd == "label" || *cmd == "call" || *cmd == "jump" ||
                   *cmd == "jzero" || *cmd == "jneg") {
            const auto label = get_word();
            if (!label) {
                cerr << "expected label\n";
                return 1;
            }
            if (*cmd == "label") {
                cout << "\n  " << str_to_ws(*label);
            } else if (*cmd == "call") {
                cout << "\n \t" << str_to_ws(*label);
            } else if (*cmd == "jump") {
                cout << "\n \n" << str_to_ws(*label);
            } else if (*cmd == "jzero") {
                cout << "\n\t " << str_to_ws(*label);
            } else {
                cout << "\n\t\t" << str_to_ws(*label);
            }
        } else if (*cmd == "ret") {
            cout << "\n\t\n";
        } else if (*cmd == "exit") {
            cout << "\n\n\n";
        } else {
            cerr << "undefined command " << *cmd << '\n';
            return 1;
        }
    }
}
