// 2008-06-26
#include <cstdio>
#ifdef _MSC_VER
#define GC getchar
#else
#define GC getchar_unlocked
#endif
int input(int& x)
{
	char c=GC();
	if (c==-1) return -1;
	else ungetc(c,stdin);
	bool minus=false;
	x=0;
	for(;;)
	{
		c=GC();
		if (c==' '||c=='\n')
			break;
		else if (c=='-')
			minus=true;
		else
			x=10*x+c-'0';
	}
	if (minus) x=-x;
	if (c==' ') return 0;
	return 1;
}
int main()
{
	static int cont[100000];
	static int item[100000];
	for(;;)
	{
		int layers=0;
		cont[0]=0;
		int x;
		bool b=true;
		for(;;)
		{
			int r=input(x);
			if (r==-1) return 0;
			if (b)
			{
				if (x<0)
				{
					item[layers]=-x;
					cont[layers++]=0;
				}
				else
				{
					if (!layers||item[layers-1]!=x)
						b=false;
					else
					{
						layers--;
						if (layers)
						{
							cont[layers-1]+=x;
							if (cont[layers-1]>=item[layers-1])
								b=false;
						}
						else if (!r)
							b=false;
					}
				}
			}
			if (r) break;
		}
		if (b&&!layers)
			printf(":-) Matrioshka!\n");
		else
			printf(":-( Try again.\n");
	}
}
