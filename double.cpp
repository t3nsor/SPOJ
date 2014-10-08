// 2009-01-16
#include <iostream>
#include <deque>
using namespace std;
bool better(deque<int> a,deque<int> b)
{
	while (a.size()&&a.front()==0) a.pop_front();
	while (b.size()&&b.front()==0) b.pop_front();
	if (!b.size()) return true;
	return a.size()<b.size()||a.size()==b.size()&&a<b;
}
int main()
{
	int T,B,i,rem,j,nj;
	cin >> T;
	while (T--)
	{
		deque<int> best; best.resize(300);
		cin >> B;
		for (i=1; i<B; i++)
		{
			rem=0;
			deque<int> cur;
			j=i;
			do
			{
				cur.push_front(j);
				j=j*2+rem;
				if (j>=B)
				{
					rem=1;
					j-=B;
				}
				else
					rem=0;
			}
			while (j!=i||rem);
			if (better(cur,best)) best=cur;
		}
		for (i=0; i<best.size()-1; i++)
			cout << best[i] << ' ';
		cout << best[best.size()-1] << endl;
	}
	return 0;
}
