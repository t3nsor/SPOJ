// 2008-06-11
#include <cstdio>
int main()
{
	int N,i,j,_count;
	int a[100];
	bool b;
	for(;;)
	{
		scanf("%d",&N);
		for (i=0; i<N; i++)
			scanf("%d",&a[i]);
		if (N==0) return 0;
		if (N%4!=2)
		{
			putchar('1');
			continue;
		}
		_count=0;
		b=false;
		for (i=1; i<N; i++)
			for (j=0; j<i; j++)
			{
				if (a[i]<a[j])
					_count++;
				if (a[i]==a[j])
					b=true;
			}
		if (b)
			putchar('1');
		else if (_count%2)
			putchar('0');
		else
			putchar('1');
	}
}
