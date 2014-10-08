// 2010-02-14
#include <iostream>
#include <set>
using namespace std;
int main()
{
	int T,N,i,j;
	int husband[500],wife[500];
	int w_pref_r[500][500];
	int m_pref[500][500];
	int proposed[500];
	scanf("%d",&T);
	while (T--)
	{
		scanf("%d",&N);
		for (i=0; i<N; i++)
		{
			scanf("%*d");
			husband[i]=-1;
			for (j=0; j<N; j++)
			{
				int x;
				scanf("%d",&x);
				w_pref_r[i][x-1]=j;
			}
		}
		set<int> free_men;
		for (i=0; i<N; i++)
		{
			scanf("%*d");
			wife[i]=-1;
			free_men.insert(i);
			proposed[i]=0;
			for (j=0; j<N; j++)
			{
				scanf("%d",m_pref[i]+j);
				m_pref[i][j]--;
			}
		}
		while (free_men.size())
		{
			int m=*(free_men.begin());
			int w=m_pref[m][proposed[m]++];
			if (husband[w]==-1)
			{
				husband[w]=m;
				wife[m]=w;
				free_men.erase(m);
			}
			else if (w_pref_r[w][husband[w]]>w_pref_r[w][m])
			{
				free_men.insert(husband[w]);
				wife[husband[w]]=-1;
				husband[w]=m;
				wife[m]=w;
				free_men.erase(m);
			}
		}
		for (i=0; i<N; i++)
			printf("%d %d\n",i+1,wife[i]+1);
	}
	return 0;
}
