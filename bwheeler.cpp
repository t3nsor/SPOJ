// 2008-08-20
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
using namespace std;
typedef pair<int,int> pii;
char grid[1000][1000];
pii arr[1000];
int N;
pii radixarr[26][1000];
void order(int sc)
{
	int i=0;
	int j,k,m,cnt;
	int lettercnt[26];
	char c;
	while (i<N)
	{
		j=i+1;
		while (j<N&&arr[i].first==arr[j].first)
			j++;
		if (j-i>1)
		{
			memset(lettercnt,0,sizeof(lettercnt));
			for (k=i; k<j; k++)
			{
				c=grid[sc][arr[k].second]-'a';
				radixarr[c][lettercnt[c]++]=arr[k];
			}
			cnt=i;
			for (k=0; k<26; k++)
				for (m=0; m<lettercnt[k]; m++)
					arr[cnt++]=radixarr[k][m];
			for (k=i+1; k<j; k++)
				if (grid[sc][arr[k].second]==grid[sc][arr[k-1].second])
					arr[k].first=arr[k-1].first;
				else
					arr[k].first=k;
		}
		i=j;
	}
}
int main()
{
	int spos,i,j;
	char s[1001];
	for(;;)
	{
		scanf("%d",&spos);
		if (!spos) return 0;
		spos--;
		getchar();
		gets(s);
		N=strlen(s);
		strcpy(grid[N-1],s);
		for (i=0; i<N; i++)
		{
			arr[i].first=0;
			arr[i].second=i;
		}
		order(N-1);
		for (i=0; i<N-1; i++)
		{
			for (j=0; j<N; j++)
				grid[i][j]=grid[(i+N-1)%N][arr[j].second];
			order(i);
		}
		for (i=0; i<N; i++)
			putchar(grid[i][spos]);
		putchar('\n');
	}
}
