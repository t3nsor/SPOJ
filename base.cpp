// 2008-08-21
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
using namespace std;
string ConvertToBase(int n,int b)
{
	if (n==0) return "0";
	string result="";
	while (n)
	{
		if (n%b<10)
			result=char(n%b+48)+result;
		else
			result=char(n%b+55)+result;
		n/=b;
	}
	return result;
}
int ValueOf(string s,int b)
{
	int result=0;
	int i;
	for (i=0; i<s.length(); i++)
		if (s[i]<='9')
			result=b*result+int(s[i]-48);
		else
			result=b*result+int(s[i]-55);
	return result;
}
void output(string S)
{
	int i;
	if (S.length()>7)
		cout << "  ERROR";
	else
	{
		for (i=0; i<7-S.length(); i++)
			cout << ' ';
		cout << S;
	}
}
int main()
{
	string S;
	int s,d;
#ifndef ONLINE_JUDGE
	freopen("base.in","r",stdin);
	freopen("base.out","w",stdout);
#endif
	while (!cin.eof())
	{
		s=0;
		cin >> S >> s >> d;
		if (!s)
			return 0;
		S=ConvertToBase(ValueOf(S,s),d);
		output(S);
		cout << endl;
	}
}
