// 2025-06-30
// This problem can be solved by formulating it as an exact cover problem,
// similar to SUDOKU. However, in Sudoku each row, column, and block has to have
// exactly one of each digit whereas in the N-queens problem only rows and
// columns have an "exactly one" constraint while diagonals have an "at most
// one" constraint. To handle "at most one" constraints we need to add a bunch
// of extra rows that contain only a single 1, meaning that we can just pick
// this row if no other row we picked has a 1 in that column (meaning that the
// diagonal is not occupied), but we have to be careful because we don't
// actually want to select those rows early on in the recursion: doing so is
// effectively guessing which set of diagonals is going to be occupied by the
// actual queens, meaning we then waste a lot of time trying to do something
// impossible. Instead, we have to modify the S heuristic so that it prefers to
// select a column of the matrix corresponding to a row or column constraint.
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
    bool preferred;
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
    // preferred columns take precedence over non-preferred columns regardless
    // of number of ones
    column_header* c = nullptr;
    for (data_object* i = h->R; i != h; i = i->R) {
        auto* cur = static_cast<column_header*>(i);
        if (c == nullptr ||
            ((cur->preferred || !c->preferred) && cur->cnt < c->cnt)) {
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
void DLX(int rows, int cols, int preferred_cols, Matrix& m, It out) {
    // Construct the linked list structure.
    data_object root;
    root.U = &root;
    root.D = &root;
    vector<column_header> header(cols);
    vector<deque<entry>> objects(cols);
    data_object* last_header = &root;
    for (int i = 0; i < cols; i++) {
        header[i].cnt = 0;
        header[i].preferred = (i < preferred_cols);
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

int matrix[2698][298];
void do_testcase(int N) {
    memset(matrix, 0, sizeof(matrix));
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            // row constraint
            matrix[N*r + c][r] = 1;
            // column constraint
            matrix[N*r + c][N + c] = 1;
            // diagonal constraints
            matrix[N*r + c][2*N + (r + c)] = 1;
            matrix[N*r + c][4*N - 1 + (r - c + N - 1)] = 1;
        }
    }
    for (int i = 0; i < 4*N - 2; i++) {
        matrix[N*N + i][2*N + i] = 1;
    }
    vector<int> rows_selected;
    for (int i = 0; i < N; i++) {
        int c; scanf("%d", &c);
        if (c == 0) continue;
        --c;
        const int r = N*i + c;
        for (int c2 = 0; c2 < 6*N - 2; c2++) {
            if (matrix[r][c2]) {
                matrix[r][c2] = 0;
                for (int r2 = 0; r2 < N*N + 4*N - 2; r2++) {
                    if (matrix[r2][c2]) {
                        memset(matrix[r2], 0, sizeof(int) * (6*N - 2));
                    }
                }
            }
        }
        rows_selected.push_back(r);
    }
    DLX(N*N + 4*N - 2, 6*N - 2, 2*N, matrix, back_inserter(rows_selected));
    sort(rows_selected.begin(), rows_selected.end());
    bool first = true;
    for (const int r : rows_selected) {
        if (r < N*N) {
            if (!first) putchar(' ');
            first = false;
            printf("%d", r % N + 1);
        }
    }
    putchar('\n');
}
int main() {
    for (;;) {
        int N; if (scanf("%d", &N) != 1) return 0;
        do_testcase(N);
    }
}
