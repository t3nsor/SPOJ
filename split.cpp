// 2024-09-02
#include <algorithm>
#include <ctype.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;
struct Window {
    char letter;
    unique_ptr<Window> left;
    unique_ptr<Window> right;
    int min_width;
    int min_height;
    int width;
    int height;
};
std::unique_ptr<Window> build_tree(const string& s, int& idx) {
    auto result = make_unique<Window>();
    result->letter = s[idx++];
    if (!isalpha(result->letter)) {
        result->left = build_tree(s, idx);
        result->right = build_tree(s, idx);
    }
    return result;
}
void calc_min(Window* w) {
    if (w->letter == '|') {
        calc_min(w->left.get());
        calc_min(w->right.get());
        w->min_width = w->left->min_width + w->right->min_width;
        w->min_height = max(w->left->min_height, w->right->min_height);
    } else if (w->letter == '-') {
        calc_min(w->left.get());
        calc_min(w->right.get());
        w->min_width = max(w->left->min_width, w->right->min_width);
        w->min_height = w->left->min_height + w->right->min_height;
    } else {
        w->min_width = w->min_height = 2;
    }
}
void stretch(int goal, int x, int y, int& newx, int& newy) {
    newy = (y * goal) / (x + y);
    newx = goal - newy;
}
void calc(Window* w) {
    if (isalpha(w->letter)) return;
    Window* l = w->left.get();
    Window* r = w->right.get();
    if (w->letter == '|') {
        stretch(w->width, l->min_width, r->min_width, l->width, r->width);
        l->height = r->height = w->height;
    } else {
        stretch(w->height, l->min_height, r->min_height, l->height, r->height);
        l->width = r->width = w->width;
    }
    calc(l);
    calc(r);
}
void overwrite(char& dest, char val) {
    if (isalpha(val)) {
        dest = val;
        return;
    }
    if (isalpha(dest)) return;
    if (val == '*') {
        dest = val;
        return;
    }
    if (dest != '*') dest = val;
}
void draw(Window* w, int r, int c, vector<string>& grid) {
    overwrite(grid[r][c], '*');
    overwrite(grid[r + w->height][c], '*');
    overwrite(grid[r][c + w->width], '*');
    overwrite(grid[r + w->height][c + w->width] ,'*');
    for (int r2 = r + 1; r2 < r + w->height; r2++) {
        overwrite(grid[r2][c], '|');
        overwrite(grid[r2][c + w->width], '|');
    }
    for (int c2 = c + 1; c2 < c + w->width; c2++) {
        overwrite(grid[r][c2], '-');
        overwrite(grid[r + w->height][c2], '-');
    }
    if (isalpha(w->letter)) {
        overwrite(grid[r][c], w->letter);
    } else if (w->letter == '|') {
        draw(w->left.get(), r, c, grid);
        draw(w->right.get(), r, c + w->left->width, grid);
    } else {
        draw(w->left.get(), r, c, grid);
        draw(w->right.get(), r + w->left->height, c, grid);
    }
}
void do_testcase() {
    string s; cin >> s;
    int idx = 0;
    auto w = build_tree(s, idx);
    calc_min(w.get());
    w->width = w->min_width;
    w->height = w->min_height;
    calc(w.get());
    vector<string> grid(w->height + 1, string(w->width + 1, ' '));
    draw(w.get(), 0, 0, grid);
    for (const auto& line : grid) cout << line << '\n';
}
int main() {
    int T; cin >> T;
    for (int i = 1; i <= T; i++) {
        cout << i << '\n';
        do_testcase();
    }
}
