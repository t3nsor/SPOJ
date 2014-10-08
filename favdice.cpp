// 2008-04-30
#include <iostream>
using namespace std;
int main()
{
	int t;
	cin >> t;
	while(t--)
	{
		double e=0.0;
		int N,j;
		cin >> N;
		for (j=1; j<=N; j++)
			e+=N/(double)j;
		printf("%.2f\n",float(e));
	}
	return 0;
}
