// 2010-11-15
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
using namespace std;
string morse[26]={
".-","-...","-.-.","-..",
".","..-.","--.","....",
"..",".---","-.-",".-..",
"--","-.","---",".--.",
"--.-",".-.","...","-",
"..-","...-",".--","-..-",
"-.--","--.."};

int main()
{
	int T;
	scanf("%d\n",&T);
	while (T--)
	{
		static char text[11111];
		static char w[11111];
		int dp[11111];
		vector<char> words[11111];
		int i,j,k,m,M;
		int N=strlen(gets(text));
		scanf("%d\n",&M);
		for (i=0; i<M; i++)
		{
			words[i].clear();
			int n=strlen(gets(w));
			int len=0;
			for (j=0; j<n; j++)
				len+=morse[w[j]-'A'].size();
			words[i].resize(len);
			for (j=0,m=0; j<n; j++)
				for (k=0; k<morse[w[j]-'A'].size(); k++)
					words[i][m++]=morse[w[j]-'A'][k];
		}
		dp[0]=1;
		for (i=1; i<=N; i++)
		{
			dp[i]=0;
			for (j=0; j<M; j++)
				if (words[j].size()<=i)
				{
					bool ok=true;
					for (k=1; k<=words[j].size(); k++)
						if (text[i-k]!=words[j].end()[-k])
						{
							ok=false;
							break;
						}
					if (ok)
						dp[i]+=dp[i-words[j].size()];
				}
		}
		printf("%d\n",dp[N]);
	}
	return 0;
}

