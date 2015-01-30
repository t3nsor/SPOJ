// 2008-10-13
#include <iostream>
using namespace std;
int main()
{
	int K; scanf("%d",&K);
	//static char field[1000];
	int histogra[1002];
	int i,j,X,Y,P,x,y,res;
	int S[1002];
	int l[1002];
	int r[1002];
	char c;
	while (K--)
	{
		scanf("%d %d",&Y,&X);
		memset(histogra,0,4*(X+2));
		res=0;
		for (i=0; i<Y; i++)
		{
			for (j=0; j<X; j++)
			{
				c=0; while (c<=32) c=getchar_unlocked();
				if (c-'F')
					histogra[j+1]=0;
				else
					histogra[j+1]++;
			}	
			histogra[0]=-1;
			histogra[X+1]=-1;
			int top=0;
			for (j=1; j<=X+1; j++)
			{
				while (top&&histogra[S[top-1]]>histogra[j])
					r[S[--top]]=j;
				S[top++]=j;
			}
			top=0;
			for (j=X; j>=0; j--)
			{
				while (top&&histogra[S[top-1]]>histogra[j])
					l[S[--top]]=j;
				S[top++]=j;
			}
			for (j=1; j<=X; j++)
				res=max(res,histogra[j]*(r[j]-l[j]-1));
		}
		printf("%d\n",3*res);
	}
	return 0;
}
