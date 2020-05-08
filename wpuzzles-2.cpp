// 2020-05-07
// Note this is an "unoptimized" Aho-Corasick.
// I'm not trying for a speed record here - AC is probably not the way.
#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
using namespace std;
struct Node {
    // for the Aho-Corasick automaton
    unique_ptr<Node> child[26] = {0};
    Node* suffix = nullptr;
    Node* next_dict = nullptr;
    int dict_id = -1;
    // sizeof(Node) is probably like 232
};
struct AC {
    Node root;

    template <class It>
    AC(It begin, It end) {
        for (int id = 0; begin != end; ++id, (void)++begin) {
            insert(id, *begin);
        }
        bfs();
    }

    void insert(int id, std::string word) {
        Node* p = &root;
        for (const char c : word) {
            auto& next = p->child[c - 'A'];
            if (!next) {
                next = make_unique<Node>();
            }
            p = next.get();
        }
        p->dict_id = id;
    }

    void bfs() {
        queue<Node*> q;
        q.push(&root);
        while (!q.empty()) {
            Node* node = q.front();
            q.pop();
            // compute `suffix` and `next_dict` for each child
            for (int i = 0; i < 26; i++) {
                if (!node->child[i]) continue;
                Node* child = node->child[i].get();
                Node* current_suffix = node->suffix;
                while (current_suffix &&
                       !current_suffix->child[i]) {
                    current_suffix = current_suffix->suffix;
                }
                child->suffix = current_suffix
                                ? current_suffix->child[i].get()
                                : &root;
                child->next_dict = child->suffix->dict_id != -1
                                   ? child->suffix
                                   : child->suffix->next_dict;
                q.push(child);
            }
        }
    }

    // Reports matches by calling
    // callback(iterator_to_last_char, dict_id) (0-based)
    template <class It, class Cb>
    void match(It begin, It end, Cb callback) {
        Node* p = &root;
        for (; begin != end; ++begin) {
            const char c = *begin;
            for (;;) {
                if (p->child[c - 'A']) {
                    p = p->child[c - 'A'].get();
                    break;
                }
                p = p->suffix;
                if (!p) {
                    p = &root;
                    break;
                }
            }
            if (p->dict_id != -1) callback(begin, p->dict_id);
            Node* q = p->next_dict;
            while (q) {
                callback(begin, q->dict_id);
                q = q->next_dict;
            }
        }
    }
};

int main() {
    int T; cin >> T;
    bool first = true;
    while (T--) {
        int R, C, W; cin >> R >> C >> W;
        vector<string> grid;
        vector<string> words;
        for (int i = 0; i < R; i++) {
            string row; cin >> row; grid.push_back(row);
        }
        for (int i = 0; i < W; i++) {
            string word; cin >> word; words.push_back(word);
        }
        AC ac(words.begin(), words.end());
        vector<int> row_found(words.size());
        vector<int> col_found(words.size());
        vector<char> dir_found(words.size());
        // Search horizontally
        for (int row = 0; row < R; row++) {
            string haystack = grid[row];
            ac.match(haystack.begin(), haystack.end(),
                     [&](auto it, int dict_id) {
                         const auto haystack_idx =
                           (it - (words[dict_id].length() - 1)) -
                           haystack.begin();
                         dir_found[dict_id] = 'C';
                         row_found[dict_id] = row;
                         col_found[dict_id] = haystack_idx;
                     });
            ac.match(haystack.rbegin(), haystack.rend(),
                     [&](auto it, int dict_id) {
                         const auto haystack_ridx =
                           (it - (words[dict_id].length() - 1)) -
                           haystack.rbegin();
                         dir_found[dict_id] = 'G';
                         row_found[dict_id] = row;
                         col_found[dict_id] =
                           haystack.length() - 1 - haystack_ridx;
                     });
        }
        // Search vertically
        for (int col = 0; col < C; col++) {
            string haystack;
            for (int i = 0; i < R; i++) {
                haystack.push_back(grid[i][col]);
            }
            ac.match(haystack.begin(), haystack.end(),
                     [&](auto it, int dict_id) {
                         const auto haystack_idx =
                           (it - (words[dict_id].length() - 1)) -
                           haystack.begin();
                         dir_found[dict_id] = 'E';
                         row_found[dict_id] = haystack_idx;
                         col_found[dict_id] = col;
                     });
            ac.match(haystack.rbegin(), haystack.rend(),
                     [&](auto it, int dict_id) {
                         const auto haystack_ridx =
                           (it - (words[dict_id].length() - 1)) -
                           haystack.rbegin();
                         dir_found[dict_id] = 'A';
                         row_found[dict_id] =
                           haystack.length() - 1 - haystack_ridx;
                         col_found[dict_id] = col;
                     });
        }
        // Search along positive diagonals
        for (int diff = R; diff >= -C; --diff) {
            const int origin_r = max(diff, 0);
            const int origin_c = max(-diff, 0);
            string haystack;
            for (int i = 0; origin_r + i < R && origin_c + i < C; i++) {
                haystack.push_back(grid[origin_r + i][origin_c + i]);
            }
            ac.match(haystack.begin(), haystack.end(),
                     [&](auto it, int dict_id) {
                         const auto haystack_idx =
                           (it - (words[dict_id].length() - 1)) -
                           haystack.begin();
                         dir_found[dict_id] = 'D';
                         row_found[dict_id] = origin_r + haystack_idx;
                         col_found[dict_id] = origin_c + haystack_idx;
                     });
            ac.match(haystack.rbegin(), haystack.rend(),
                     [&](auto it, int dict_id) {
                         const auto haystack_ridx =
                           (it - (words[dict_id].length() - 1)) -
                           haystack.rbegin();
                         dir_found[dict_id] = 'H';
                         row_found[dict_id] =
                           origin_r + (haystack.length() - haystack_ridx - 1);
                         col_found[dict_id] =
                           origin_c + (haystack.length() - haystack_ridx - 1);
                     });
        }
        // Search along negative diagonals
        for (int sum = 0; sum <= R + C - 2; ++sum) {
            const int origin_r = min(R - 1, sum);
            const int origin_c = max(0, sum - (R - 1));
            string haystack;
            for (int i = 0; origin_r - i >= 0 && origin_c + i < C; i++) {
                haystack.push_back(grid[origin_r - i][origin_c + i]);
            }
            ac.match(haystack.begin(), haystack.end(),
                     [&](auto it, int dict_id) {
                         const auto haystack_idx =
                           (it - (words[dict_id].length() - 1)) -
                           haystack.begin();
                         dir_found[dict_id] = 'B';
                         row_found[dict_id] = origin_r - haystack_idx;
                         col_found[dict_id] = origin_c + haystack_idx;
                     });
            ac.match(haystack.rbegin(), haystack.rend(),
                     [&](auto it, int dict_id) {
                         const auto haystack_ridx =
                           (it - (words[dict_id].length() - 1)) -
                           haystack.rbegin();
                         dir_found[dict_id] = 'F';
                         row_found[dict_id] =
                           origin_r - (haystack.length() - haystack_ridx - 1);
                         col_found[dict_id] =
                           origin_c + (haystack.length() - haystack_ridx - 1);
                     });
        }
        if (!first) putchar('\n');
        for (int i = 0; i < W; i++) {
            printf("%d %d %c\n", row_found[i], col_found[i], dir_found[i]);
        }
        first = false;
    }
}
