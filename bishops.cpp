// 2008-06-19
#include <iostream>
#include <cstring>
using namespace std;
#define MAX(x,y) ((x)>(y)?(x):(y))
void swap(char&x,char&y)
{
	char t=x;
	x=y;
	y=t;
}
int main()
{
	char s[110];
	int i;
	for(;;)
	{
		scanf("%s",&s);
		int l=strlen(s);
		if (l==0)
			return 0;
		if (l==1&&s[0]=='0')
		{
			printf("0\n");
			s[0]=0;
			continue;
		}
		if (l==1&&s[0]=='1')
		{
			printf("1\n");
			s[0]=0;
			continue;
		}
		for (i=0; i<l/2; i++)
			swap(s[i],s[l-i-1]);
		int carry=0;
		for (i=0; i<l; i++)
		{
			s[i]=2*(s[i]-'0')+carry;
			if (s[i]>=10)
			{
				carry=1;
				s[i]-=10;
			}
			else
				carry=0;
		}
		if (carry)
			s[l++]=1;
		bool borrow;
		s[0]-=2;
		if (s[0]<0)
		{
			borrow=true;
			s[0]+=10;
		}
		else
			borrow=false;
		i=1;
		while (borrow)
		{
			s[i]--;
			if (s[i]==-1)
			{
				s[i]=9;
				borrow=true;
			}
			else
				borrow=false;
			i++;
		}
		if (s[l-1]==0)
			l--;
		for (i=l-1; i>=0; i--)
			putchar(s[i]+'0');
		putchar('\n');
		s[0]=0;
	}
}
