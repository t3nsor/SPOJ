// 2008-04-19
#include <iostream>
#define MAX 34000
using namespace std;
int main()
{
	static bool a[MAX];
	int b[3000];
	int i,j,k,m,n,N;
	memset(a,true,sizeof(a));
	j=2;
	n=0;
	for (i=0; i<3000; i++)
	{
		while (!a[j]) j++;
		a[j]=false;
		b[n++]=j;
		//cout << j << endl;
		k=j;
		while (k<MAX)
		{
			for (m=0; m<j; m++)
			{
				k++;
				while ((k<MAX)&&(!a[k]))
					k++;
				if (k==MAX)
					break;
			}
			if (k==MAX) break;
			a[k]=false;
		}
		j++;
	}
	for(;;)
	{
		cin >> N;
		if (!N) break;
		cout << b[N-1] << endl;
	}
	return 0;
}
