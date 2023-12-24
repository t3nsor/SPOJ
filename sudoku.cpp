// 2023-12-23
// Knuth's DLX algorithm with the S heuristic.
#include <algorithm>
#include <deque>
#include <iterator>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;
// Individual object in the sparse matrix structure. Can be the root, a
// column header, or a 1 entry.
struct data_object {
    data_object* L;  // left link
    data_object* R;  // right link
    data_object* U;  // up link
    data_object* D;  // down link
};
struct column_header : data_object {
    int cnt;  // number of ones in this column
    // cover the column headed by this object
    void cover();
    // uncover the column headed by this object
    void uncover();
};
struct entry : data_object {
    column_header* header;
    int row;
};
void column_header::cover() {
    data_object* i = D;
    R->L = L;
    L->R = R;
    while (i != this) {
        data_object* j = i->R;
        while (j != i) {
            j->D->U = j->U;
            j->U->D = j->D;
            static_cast<entry*>(j)->header->cnt--;
            j = j->R;
        }
        i = i->D;
    }
}

void column_header::uncover() {
    data_object* i = U;
    while (i != this) {
        data_object* j = i->L;
        while (j != i) {
            static_cast<entry*>(j)->header->cnt++;
            j->D->U = j;
            j->U->D = j;
            j = j->L;
        }
        i = i->U;
    }
    R->L = this;
    L->R = this;
}

template <class It>
bool DLX_search(data_object* h, vector<int>& stk, It out) {
    if (h->R == h) {
        // done!
        copy(stk.begin(), stk.end(), out);
        return true;
    }
    // find column with fewest ones; break tie by column ID
    column_header* c = nullptr;
    for (data_object* i = h->R; i != h; i = i->R) {
        auto* cur = static_cast<column_header*>(i);
        if (c == nullptr || cur->cnt < c->cnt) {
            c = cur;
        }
    }
    c->cover();
    data_object* r = c->D;
    data_object* j;
    while (r != c) {
        stk.push_back(static_cast<entry*>(r)->row);
        j = r->R;
        while (j != r) {
            static_cast<entry*>(j)->header->cover();
            j = j->R;
        }
        if (DLX_search(h, stk, out)) {
            return true;
        }
        //set r <- stk.top(), and c<- C[r], this is unnecessary
        stk.pop_back();
        j = r->L;
        while (j != r) {
            static_cast<entry*>(j)->header->uncover();
            j = j->L;
        }
        r = r->D;
    }
    c->uncover();
    return false;
}
template <class Matrix, class It>
void DLX(int rows, int cols, Matrix& m, It out) {
    // Construct the linked list structure.
    data_object root;
    root.U = &root;
    root.D = &root;
    vector<column_header> header(cols);
    vector<deque<entry>> objects(cols);
    data_object* last_header = &root;
    for (int i = 0; i < cols; i++) {
        header[i].cnt = 0;
        for (int j = 0; j < rows; j++) {
            if (!m[j][i]) continue;
            objects[i].emplace_back();
            entry& cur = objects[i][objects[i].size() - 1];
            if (objects[i].size() > 1) {
                entry& prev = objects[i][objects[i].size() - 2];
                prev.D = &cur;
                cur.U = &prev;
            }
            cur.header = &header[i];
            cur.row = j;
            header[i].cnt++;
        }
        if (header[i].cnt) {
            // complete the circular linked list
            header[i].D = &objects[i].front();
            objects[i].front().U = &header[i];
            header[i].U = &objects[i].back();
            objects[i].back().D = &header[i];
            // insert this column header
            header[i].L = last_header;
            last_header = last_header->R = &header[i];
        }
        // else: this column is empty; skip it
    }
    // complete the circular linked list
    root.L = last_header;
    last_header->R = &root;
    // construct L/R links for the data objects
    vector<data_object*> p(cols);
    for (int i = 0; i < cols; i++) {
        p[i] = &header[i];
    }
    for (int i = 0; i < rows; i++) {
        data_object head;
        head.R = &head;
        data_object* tail = &head;
        for (int j = 0; j < cols; j++) {
            if (!m[i][j]) continue;
            p[j] = p[j]->D;
            p[j]->L = tail;
            tail = tail->R = p[j];
        }
        data_object* first = head.R;
        first->L = tail;
        tail->R = first;
    }
    vector<int> stk;
    DLX_search(&root, stk, out);
}

int matrix[4096][1024];
void do_testcase() {
    memset(matrix, 0, sizeof(matrix));
    char grid[16][17];
    for (int row = 0, r = 0; r < 16; r++) {
        for (int c = 0; c < 16; c++) {
            for (int i = 0; i < 16; i++,row++) {
                // uniqueness constraint
                matrix[row][r + 16*c] = 1;
                // row constraint
                matrix[row][256 + i + 16*r] = 1;
                // column constraint
                matrix[row][512 + i + 16*c] = 1;
                // block constraint
                int block = 4 * (r/4) + (c/4);
                matrix[row][768 + i + 16*block]=1;
            }
        }
    }
    for (int r = 0; r < 16; r++) {
        for (int c = 0; c < 16; c++) {
            char ch;
            scanf("%c", &ch);
            if (ch >= 'A' && ch <= 'Z') {
                ch -= 'A';
                const int row = 256*r + 16*c + ch;
                for (int col = 0; col < 1024; col++) {
                    if (matrix[row][col]) {
                        matrix[row][col] = 0;
                        for (int row2 = 0; row2 < 4096; row2++) {
                            if (matrix[row2][col]) {
                                matrix[row2][col] = 0;
                                memset(matrix[row2], 0, sizeof(matrix[row2]));
                            }
                        }
                    }
                }
                grid[r][c] = ch + 'A';
            } else if (ch != '-') {
                c--;
            }
        }
        grid[r][16] = 0;
    }
    vector<int> rows_selected;
    DLX(4096, 1024, matrix, back_inserter(rows_selected));
    for (const int x : rows_selected) {
        grid[x / 256][x / 16 % 16] = x % 16 + 'A';
    }
    for (const char* row : grid) {
        puts(row);
    }
    putchar('\n');
}
int main() {
    int T; scanf("%d",&T);
    while (T--) do_testcase();
}
