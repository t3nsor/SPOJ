// 2008-04-20
#include <iostream>
using namespace std;
int A[10];
void process(int i)
{
	while (i)
	{
		A[i%10]++;
		i/=10;
	}
}
int main()
{
	int a,b,i,j;
	for(j=1;;j++)
	{
		memset(A,0,sizeof(A));
		cin >> a;
		if (a==0) return 0;
		cin >> b;
		if (a>b) swap(a,b);
		for (i=a; i<=b; i++)
			process(i);
		cout << "Case " << j << ": ";
		for (i=0; i<10; i++)
		{
			cout << i << ':' << A[i];
			if (i<9)
				cout << ' ';
			else
				cout << endl;
		}
	}
}
