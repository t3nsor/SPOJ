// 2008-07-07
#include <iostream>
#include <queue>
using namespace std;
struct job
{
	int priority;
	int position;
};
int main()
{
	int t;
	job J;
	cin >> t;
	int i,j,k,m,n,p;
	int P[10];
	for (i=0; i<t; i++)
	{
		queue<job> Q;
		cin >> n >> m;
		memset(P,0,sizeof(P));
		for (j=0; j<n; j++)
		{
			cin >> p;
			J.position=j;
			J.priority=p;
			P[p]++;
			Q.push(J);
		}
		int biggest=9;
		while (!P[biggest]) biggest--;
		int _time=0;
		for(;;)
		{
			J=Q.front();
			Q.pop();
			if (J.priority<biggest)
				Q.push(J);
			else
			{
				P[J.priority]--;
				while (!P[biggest]) biggest--;
				_time++;
				if (J.position==m)
					break;
			}
		}
		cout << _time << endl;
	}
	return 0;
}
