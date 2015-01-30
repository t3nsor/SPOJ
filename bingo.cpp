// 2008-08-21
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cctype>
#include <string>
#include <set>
using namespace std;
string get()
{
	char c;
	string res="";
	for(;;)
	{
		c=getchar();
		if (c==-1||c==EOF)
			return res;
		if (isalpha(c))
		{
			ungetc(c,stdin);
			break;
		}
	}
	for(;;)
	{
		c=getchar();
		if (isalpha(c))
			res+=tolower(c);
		else
			return res;
	}
}
int gcd(int x,int y)
{
	if (x<y)
		return gcd(y,x);
	if (y==0)
		return x;
	return gcd(x%y,y);
}
int main()
{
#ifndef ONLINE_JUDGE
	freopen("bingo.in","r",stdin);
#endif
	set<string> S;
	string s;
	int games=0;
	int total=0;
	for(;;)
	{
		s=get();
		if (!s.length())
			break;
		if (s=="bullshit")
		{
			total+=S.size();
			S.clear();
			games++;
		}
		else
			S.insert(s);
	}
	int x=gcd(games,total);
	printf("%d / %d\n",total/x,games/x);
	return 0;
}
