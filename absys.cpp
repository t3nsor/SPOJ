// 2008-06-16
#include <iostream>
#include <cstring>
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAXS 10000
using namespace std;
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
		if (length==0) {digits[0]=0; length=1;}
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
	void print()
	{
		int i;
		for (i=length-1; i>=0; i--)
			putchar(digits[i]+48);
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
int main()
{
	int i,j,T;
	scanf("%d",&T);
	char s1[MAXS],s2[MAXS],s3[MAXS];
	bignum n1(MAXS),n2(MAXS),n3(MAXS);
	for (j=0; j<T; j++)
	{
		scanf("%s + %s = %s",&s1,&s2,&s3);
		//one of the numbers has a "machula"
		for (i=0; i<strlen(s1); i++)
			if (s1[i]=='m')
			{
				n2=s2;
				n3=s3;
				n1=n3;
				n1.sub(n2);
			}
		for (i=0; i<strlen(s2); i++)
			if (s2[i]=='m')
			{
				n1=s1;
				n3=s3;
				n2=n3;
				n2.sub(n1);
			}
		for (i=0; i<strlen(s3); i++)
			if (s3[i]=='m')
			{
				n1=s1;
				n2=s2;
				n3=n1;
				n3.add(n2);
			}
		n1.print(); printf(" + "); n2.print(); printf(" = "); n3.print(); printf("\n");
	}
	return 0;
}
