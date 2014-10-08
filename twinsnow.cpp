// 2009-05-12
// modified 2014-10-07
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <functional>
using namespace std;
struct Snowflake
{
	int x[6];
};
bool operator<(const Snowflake& X1,const Snowflake& X2)
{
	return lexicographical_compare(X1.x,X1.x+6,X2.x,X2.x+6);
}
bool operator==(const Snowflake& X1,const Snowflake& X2)
{
	return equal(X1.x,X1.x+6,X2.x);
}
void canonicalize(Snowflake& x)
{
	Snowflake y=x;
	int i;
	for (i=0; i<6; i++)
	{
		rotate(y.x,y.x+1,y.x+6);
		x=min(x,y);
	}
	reverse(y.x,y.x+6);
	for (i=0; i<6; i++)
	{
		rotate(y.x,y.x+1,y.x+6);
		x=min(x,y);
	}
}
int in()
{
	char c;
	int x=0;
	for (;;)
	{
		c=getchar_unlocked();
		if (c<=32) return x;
		x=(x<<1)+(x<<3)+c-'0';
	}
}
int main()
{
	int n,i;
	n=in();
	vector<Snowflake> V;
	Snowflake s;
	bool b=false;
	while (n--)
	{
		for (i=0; i<6; i++)
			s.x[i]=in();
	//	canonicalize(s);
		sort(s.x,s.x+6);
		V.push_back(s);
	}
	sort(V.begin(),V.end());
	for (i=1; i<V.size(); i++)
		if (V[i]==V[i-1])
		{
			printf("Twin snowflakes found.\n");
			return 0;
		}
	printf("No two snowflakes are alike.\n");
	return 0;
}
