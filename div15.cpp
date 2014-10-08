// 2009-04-19
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <cassert>
#include <string.h>
using namespace std;

int fox(int a,int b)
{
	int tot=0,n1=a,n2=b;
		while ((n1>2) && (n1>n2+1))
			tot+=3,n1-=3;
		while ((n2>2) && (n2>n1+1))
			tot+=3,n2-=3;
	return(tot+min(n1,n2)*2);
}

int main()
{
	//freopen("spoj.txt","r",stdin);
	//vars
	int t=10,a,b;
	bool zero;
	static char s[3005];
	int ndig[3];
	static char dig[3][3005];
	int nout;
	static char out[3005];
	//testcase loop
	scanf("%d\n",&t);
		while (t--)
		{
			//input
			scanf("%s",s);
			//separate by %3
			ndig[0]=ndig[1]=ndig[2]=0;
			zero=0;
				for (a=strlen(s)-1; a>=0; a--)
				{
					if ((s[a]=='0') && (!zero))
						zero=1;
					else
					{
						assert(s[a]>=48);
						b=(s[a]-'0')%3;
						dig[b][ndig[b]++]=s[a];
					}
				}
			//sort
				for (a=0; a<3; a++)
					sort(dig[a],dig[a]+ndig[a]);
			nout=0;
			//find last digit
				if (!zero)
				{
					//no 0s, so look for a 5
						for (a=0; a<ndig[2]; a++)
						{
							if (dig[2][a]=='5')
							{
								//take the 5
								memmove(dig[2]+a,dig[2]+a+1,sizeof(int)*(ndig[2]-a-1));
								ndig[2]--;
								//take other digits to cancel it out
									if ((ndig[1]>0) && (fox(ndig[1]-1,ndig[2])>fox(ndig[1],ndig[2]-2)))
									{
										out[nout++]=dig[1][--ndig[1]];
									}
									else
									if (ndig[2]>1)
									{
										out[nout++]=dig[2][--ndig[2]];

										out[nout++]=dig[2][--ndig[2]];

									}
									else
										goto bad;
								goto ok;
							}
						}
					//no 0s or 5s
bad:
					printf("impossible\n");
					continue;
				}
ok:
			//use as many digits as possible
				for (a=0; a<ndig[0]; a++)
				{
					out[nout++]=dig[0][a];

				}
				while ((ndig[1]>2) && (ndig[1]>ndig[2]+1))
					for (a=0; a<3; a++)
					{

						out[nout++]=dig[1][--ndig[1]];

					}
				while ((ndig[2]>2) && (ndig[2]>ndig[1]+1))
					for (a=0; a<3; a++)
					{

						out[nout++]=dig[2][--ndig[2]];

					}
				for (a=min(ndig[1],ndig[2]); a--; )
				{

					out[nout++]=dig[1][--ndig[1]];
					out[nout++]=dig[2][--ndig[2]];

				}
			//sort and output
				if (nout<1)
					goto grr;
			sort(out,out+nout);
				if (out[nout-1]=='0')
				{
grr:
					printf("0\n");
					continue;
				}
				for (a=nout-1; a>=0; a--)
				{
					printf("%c",out[a]);
				}
				if (zero)
					printf("0\n");
				else
					printf("5\n");
		}
}
