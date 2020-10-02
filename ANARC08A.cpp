/*
USER: zobayer
TASK: ANARC08A
ALGO: breadth first search
*/

#include <cstdio>
#include <cstring>
#include <cassert>
#include <queue>
using namespace std;

struct trie {
	int idx;
	trie *next[9];
	trie() { for(int i=0; i<9; i++) next[i]=NULL; idx = -1; }
} *T;

inline int find(char *s) {
	trie *curr = T; int i, k;
	for(i = 0; s[i]; i++) {
		k = s[i] - '1';
		if(curr->next[k] == NULL) return -1;
		curr = curr->next[k];
	}
	return curr->idx;
}

inline int check(char *s, int &p) {
	trie *curr = T; int i, k;
	for(i = 0; s[i]; i++) {
		k = s[i] - '1';
		if(curr->next[k] == NULL) curr->next[k] = new trie;
		curr = curr->next[k];
	}
	if(curr->idx == -1) curr->idx = p++;
	return curr->idx;
}

int moves[8][4] = {
	{0,3,4,1}, {1,4,5,2}, {3,6,7,4}, {4,7,8,5},
	{0,1,4,3}, {1,2,5,4}, {3,4,7,6}, {4,5,8,7}
};

const int MAX = 360980;

char s[MAX][10] = {"123456789"};
int d[MAX];

void bfs() {
	int u, v, i, j, k, p;
	char temp[10] = {0}, x;
	queue< int > Q;
	Q.push(0); d[0] = k = 0;
	check(s[0], k);
	while(!Q.empty()) {
		u = Q.front(); Q.pop();
		if(d[u] >= 9) continue;
		for(i = 0; i < 8; i++) {
			strcpy(temp, s[u]);
			for(j = 1, x = temp[moves[i][0]]; j < 4; j++)
				temp[moves[i][j-1]] = temp[moves[i][j]];
			temp[moves[i][j-1]] = x;
			p = k; v = check(temp, k);
			if(p != k) {
				d[v] = d[u] + 1;
				Q.push(v);
				strcpy(s[v], temp);
			}
		}
	}
}

int main() {
	char inp[12];
	int f, k, cs = 1;
	T = new trie;
	bfs();
	while(scanf("%s", inp)==1 && inp[1]>'0') {
		k = inp[0] - '0';
		f = find(&inp[1]);
		if(f == -1 || d[f] > k) printf("%d. -1\n", cs++);
		else printf("%d. %d\n", cs++, d[f]);
	}
	return 0;
}
