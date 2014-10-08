// 2008-08-21
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
using namespace std;
int main()
{
	int k1,k2,k3,N,i,j,cnt;
	char s[100];
	char s2[100];
	for(;;)
	{
		scanf("%d %d %d",&k1,&k2,&k3);
		if (!(k1|k2|k3))
			return 0;
		scanf("%s",s);
		N=strlen(s);
		s2[N]=0;
		for (i=0; i<N; i++)
		{
			if (s[i]>='a'&&s[i]<='i')
			{
				j=i;
				cnt=0;
				while (cnt<k1)
				{
					j=(j+1)%N;
					if (s[j]>='a'&&s[j]<='i')
						cnt++;
				}
				s2[j]=s[i];
			}
			else if (s[i]>='j'&&s[i]<='r')
			{
				j=i;
				cnt=0;
				while (cnt<k2)
				{
					j=(j+1)%N;
					if (s[j]>='j'&&s[j]<='r')
						cnt++;
				}
				s2[j]=s[i];
			}
			else
			{
				j=i;
				cnt=0;
				while (cnt<k3)
				{
					j=(j+1)%N;
					if (s[j]>='s'&&s[j]<='z'||s[j]=='_')
						cnt++;
				}
				s2[j]=s[i];
			}
		}
		printf("%s\n",s2);
	}
}
