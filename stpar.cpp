// 2008-07-07
#include <cstdio>
#include <stack>
using namespace std;
int main()
{
	int n,i;
	int l[2000];
	int N;
	for(;;)
	{
		scanf("%d",&n);
		stack<int> S;
		if (n==0) return 0;
		for (i=0; i<n; i++)
			scanf("%d",l+i);
		int next=1;
		N=0;
		bool b=true;
		for (i=0; i<n||S.size();)
			if (S.size()&&S.top()==next)
			{
				S.pop();
				next++;
			}
			else if (l[i]>next)
			{
				if (S.size()&&S.top()<l[i])
				{
					b=false;					
					break;
				}
				else
					S.push(l[i]);
				i++;
			}
			else
			{
				i++;
				next++;
			}
		if (!b)
			printf("no\n");
		else
			printf("yes\n");
	}
}
