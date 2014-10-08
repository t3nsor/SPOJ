// 2009-02-28
//misere play Nim
#include <iostream>
using namespace std;
int get()
{
	char c;
	int x=0;
	do
	{
		c=getchar_unlocked();
	}
	while (c<=32);
	do
	{
		x=(x<<1)+(x<<3)+c-'0';
		c=getchar_unlocked();
	}
	while (c>32);
	return x;
}
int main()
{
	int T,N,i,x,nimber;
	bool onesonly;
	T=get();
	while (T--)
	{
		N=get();
		onesonly=true;
		nimber=0;
		for (i=0; i<N; i++)
		{
			x=get();
			if (x>1) onesonly=false;
			nimber^=x;
		}
		puts(!nimber^onesonly?"Brother":"John");
	}
	return 0;
}
