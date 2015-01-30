// 2008-06-16
#include <cstdio>
#include <cstring>
#include <set>
using namespace std;
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
struct bignum
{
	char* digits;
	int length;
	bignum(int size)
	{
		length=1;
		digits=new char[size];
		digits[0]=0;
	}
	bignum(int size,const bignum& x)     //Copy constructor
	{
		length=x.length;
		digits=new char[size];
		memcpy(digits,x.digits,x.length);
	}
	bignum(int size,int x)
	{
		length=0;
		digits=new char[size];
		while (x>0)
		{
			digits[length++]=x%10;
			x/=10;
		}
		if (length==0) {digits[0]=0; length=1;}
	}
	bignum(int size,char* x)
	{
		digits=new char[size];
		length=0;
		while (x[length]) length++;
		int i;
		for (i=0; i<length; i++)
			digits[i]=x[length-i-1]-'0';
	}
	~bignum()
	{
		delete digits;
	}
	void add(bignum& x)
	{
		int l=MAX(length,x.length);
		int d;
		int carry=0;
		x.digits[x.length]=0;
		memset(digits+length,0,l-length+1);
		for (d=0; d<=l; d++)
		{
			int sum=carry;
			if (d<length)
				sum+=digits[d];
			if (d<x.length)
				sum+=x.digits[d];
			digits[d]=sum;
			if (digits[d]>=10) {digits[d]-=10;carry=1;} else carry=0;
		}
		length=l;
		if (digits[length]) length++;
	}
	void sub(bignum& x)
	{
		int d;
		int borrow=0;
		digits[length]=0;
		for (d=0; d<=length; d++)
		{
			digits[d]-=borrow;
			if (d<x.length)
				digits[d]-=x.digits[d];
			if (digits[d]<0) {digits[d]+=10;borrow=1;} else borrow=0;
		}
		while (length>0&&!digits[length-1]) length--;
		if (length==0) length=1;
	}
	void operator+=(bignum& y)
	{
		add(y);
	}
	void operator-=(bignum& y)
	{
		sub(y);
	}
	void operator=(const bignum& y)
	{
		length=y.length;
		memcpy(digits,y.digits,y.length);
	}
	void operator=(const char* s)
	{
		length=strlen(s);
		int i=length;
		while (i--)
			digits[i]=s[length-i-1]-48;
	}
};
bool operator<(const bignum& x,const bignum& y)
{
	int i;
	if (x.length<y.length)
		return true;
	if (x.length>y.length)
		return false;
	for (i=x.length-1; i>=0; i--)
		if (x.digits[i]<y.digits[i])
			return true;
		else if (x.digits[i]>y.digits[i])
			return false;
	return false;
}
int main()
{
	int n,i,j;
	char K[125];
	scanf("%d",&n); n/=2;
	scanf("%s",&K);
	bignum k(125,K);
	//a[i][j] = # of Correct Bracket Expressions of length 2i, starting with j specified
	//open brackets i.e. a[2][1] = # of (.. = # of [..
	//a[i][0] = total # of CBS of length i
	bignum* a[126][126];
	a[0][0]=new bignum(125,1);
	for (i=1; i<=n; i++)
	{
		a[i][i]=new bignum(125,1);
		for (j=i-1; j>=1; j--)
		{
			a[i][j]=new bignum(125,*(a[i-1][j-1]));
			a[i][j]->add(*(a[i][j+1]));
			a[i][j]->add(*(a[i][j+1]));
		}
		a[i][0]=new bignum(125,*(a[i][1]));
		a[i][0]->add(*(a[i][1]));
		for (j=i+1; j<=i; j++)
			a[i][j]=new bignum(125,0);
	}
	bignum one(125,1);
	bignum zero(125,0);
	char answer[251];
	k-=one;
	char st[251];
	int stlen=0;
	int len2=n;
	int br=0;
	bignum ob1(125);
	for (i=0; i<2*n; i++)
	{
		//we can add (, ), [, ]
		if (br<len2)
			ob1=*(a[len2][br+1]);
		else
			ob1=zero;
		if (k<ob1)
		{
			answer[i]='(';
			st[stlen++]='(';
			br++;
			continue;
		}
		k-=ob1;
		bignum cb(125);
		if (stlen>0&&st[stlen-1]=='(')
		{
			cb=*(a[len2-1][br-1]);
			if (k<cb)
			{
				answer[i]=')';
				stlen--;
				br--;
				len2--;
				continue;
			}
			k-=cb;
		}
		if (k<ob1)
		{
			answer[i]='[';
			st[stlen++]='[';
			br++;
			continue;
		}
		k-=ob1;
		//if you're still alive, 
		answer[i]=']';
		stlen--;
		br--;
		len2--;
	}
	answer[2*n]=0;
	printf("%s\n",answer);
	return 0;
}
