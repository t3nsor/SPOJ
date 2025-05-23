// 2025-05-22
// This problem is not difficult other than that the time limit is a bit strict
// so we convert each tag name into an integer ID (assigned in increasing order)
// as soon as we read it from the input.
#include <ctype.h>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;
unordered_map<string, int> strings;
struct XML {
    int tag;
    vector<XML> children;
    unordered_map<int, int> lookup;  // index into `children`
};
char getc() {
    for (;;) {
        char c = getchar_unlocked();
        if (isalpha(c) || c == '<' || c == '>' || c == '/') return c;
    }
}
pair<int, bool> getTag() {
    getc();  // consume opening `<`
    string tagName;
    bool isEnd = false;
    for (;;) {
        char c = getc();
        if (c == '/') isEnd = true;
        else if (c == '>') break;
        else tagName.push_back(c);
    }
    auto it = strings.find(tagName);
    if (it == strings.end()) {
        const auto idx = strings.size();
        strings[tagName] = idx;
        return make_pair(idx, isEnd);
    } else {
        return make_pair(it->second, isEnd);
    }
}
XML getXML(int initialTag) {
    XML result;
    result.tag = initialTag;
    for (;;) {
        auto nextTag = getTag();
        if (nextTag.second) {
            if (nextTag.first != result.tag) throw;
            return result;
        }
        result.lookup[nextTag.first] = result.children.size();
        result.children.push_back(getXML(nextTag.first));
    }
}
bool match(const XML& root, const XML& pattern) {
    if (pattern.tag != root.tag) return false;
    for (const auto& child : pattern.children) {
        auto it = root.lookup.find(child.tag);
        if (it == root.lookup.end() ||
            !match(root.children[it->second], child)) return false;
    }
    return true;
}
void dfs(const XML& root, const XML& pattern, int& idx, vector<int>& result) {
    ++idx;
    if (match(root, pattern)) result.push_back(idx);
    for (const auto& child : root.children) {
        dfs(child, pattern, idx, result);
    }
}
int main() {
    auto rootOpen = getTag();
    XML root = getXML(rootOpen.first);
    auto patternOpen = getTag();
    const XML pattern = getXML(patternOpen.first);
    int idx = 0;
    vector<int> result;
    dfs(root, pattern, idx, result);
    printf("%d\n", (int)result.size());
    for (const auto match : result) printf("%d\n", match);
}
