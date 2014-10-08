// 2008-05-09
#include <iostream>
#include <algorithm>
using namespace std;
int main()
{
	int T;
	cin >> T;
	int i;
	int amplifier[100000];
	int a[100000];
	for (i=0; i<T; i++)
	{
		int Ni,j,ones=0;
		cin >> Ni;
		for (j=0; j<Ni; j++)
		{
			int n;
			cin >> n;
			if (n==1) ones++; else amplifier[j-ones]=n;
		}
		Ni-=ones;
		sort(amplifier,amplifier+Ni);
		if (Ni==2 && amplifier[0]==2 && amplifier[1]==3);
		else
			reverse(amplifier,amplifier+Ni);
		for (j=0; j<ones; j++)
			a[j]=1;
		for (j=ones; j<Ni+ones; j++)
			a[j]=amplifier[j-ones];
		for (j=0; j<Ni+ones-1; j++)
			cout << a[j] << ' ';
		cout << a[Ni+ones-1] << endl;
	}
	return 0;
}
