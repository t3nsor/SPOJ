// 2008-02-02
// modified 2014-10-07
#include <cstdio>
#include <cstring>
typedef long long int64;
int64 a[4]={-58,192,442,692};
int64 in()
{
	char s[20];
	scanf("%s",&s);
	int l=strlen(s);
	int i;
	int64 result=0;
	for (i=0; i<l; i++)
		result=10*result+s[i]-48;
	return result;
}
void out(int64 x)
{
	if (x==0) {printf("0\n"); return;}
	int digits=0;
	char a[20];
	int i;
	while (x>0)
	{
		a[digits++]=x%10;
		x/=10;
	}
	for (i=digits-1; i>=0; i--)
		putchar(a[i]+48);
	putchar('\n');
}
int main()
{
	int t,i;
	int64 k;
	scanf("%d",&t);
	for (i=0; i<t; i++)
	{
		scanf("%lld", &k);
		printf("%lld\n", 1000*(k/4)+a[k%4]);
	}
	return 0;
}
