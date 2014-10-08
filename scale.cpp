// 2008-06-06
#include <iostream>
using namespace std;
int main()
{
	int N,i,X,x;
	cin >> N >> X;
	int a[20];
	int b[20];
	int c[20];
	int B,C;
	x=1;
	for (i=0; i<N-1; i++)
		x*=3;
	for (i=N-1; i>=0; i--,x/=3)
		if (X==0)
			a[i]=0;
		else if (X<0&&-X>x/2)
		{
			X+=x;
			a[i]=-1;
		}
		else if (X>0&&X>x/2)
		{
			X-=x;
			a[i]=1;
		}
	if (X)
		cout << -1 << endl;
	else
	{
		B=C=0;
		for (i=0; i<N; i++)
			if (a[i]==1)
				b[B++]=i+1;
			else if (a[i]==-1)
				c[C++]=i+1;
		if (C==0)
			cout << endl;
		else
		{
			for (i=0; i<C-1; i++)
				cout << c[i] << ' ';
			cout << c[C-1] << endl;
		}
		if (B==0)
			cout << endl;
		else
		{
			for (i=0; i<B-1; i++)
				cout << b[i] << ' ';
			cout << b[B-1] << endl;
		}
	}
	return 0;
}
