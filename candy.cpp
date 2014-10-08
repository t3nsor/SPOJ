// 2008-04-19
#include <iostream>
#include <algorithm>
using namespace std;
int main()
{
	static int a[10000];
	int N,i;
	for(;;)
	{
		cin >> N;
		if (N==-1) return 0;
		int sum=0;
		for (i=0; i<N; i++)
		{
			cin >> a[i];
			sum+=a[i];
		}
		if (sum%N)
		{
			cout << "-1\n";
			continue;
		}
		sort(a,a+N);
		int last=N-1;
		int first=0;
		int _count=0;
		while (a[last]>a[first])
		{
			while (a[first]<sum/N && a[last]>sum/N)
			{
				a[first]++;
				a[last]--;
				_count++;
			}
			if (a[first]>=sum/N)
				first++;
			else
				last--;
		}
		cout << _count << endl;
	}
}
