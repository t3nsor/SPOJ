// 2008-07-22
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
#define SIZE 1000
inline int max(int a,int b)
{
	return a>b?a:b;
}
inline int min(int a,int b)
{
	return a<b?a:b;
}
struct bignum
{
	char* digits;
	int length;
	bignum()
	{
		length=1;
		digits=new char[SIZE];
		digits[0]=0;
	}
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
		int l=max(length,x.length);
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
	void addzero()
	{
		add(*this);
		bignum _this(this->length+1,*this);
		add(*this);
		add(*this);
		add(_this);
	}
	void mult(bignum& x)
	{
		bignum _this(this->length+x.length+1,*this);
		bignum __this(this->length+x.length+1);
		length=1; digits[0]=0;
		int i,j;
		for (i=0; i<x.length; i++)
		{
			__this.length=1; __this.digits[0]=0;
			for (j=0; j<x.digits[i]; j++)
				__this.add(_this);
			add(__this);
			_this.addzero();
		}
	}
	void print()
	{
		int i;
		for (i=length-1; i>=0; i--)
			putchar(digits[i]+48);
	}
	void operator+=(bignum& y)
	{
		add(y);
	}
	void operator-=(bignum& y)
	{
		sub(y);
	}
	void operator*=(bignum& x)
	{
		mult(x);
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
	int i,t,k;
#ifndef ONLINE_JUDGE
	freopen("arith.in","r",stdin);
	freopen("arith.out","w",stdout);
#endif
	scanf("%d",&t);
	char c;
	char s[1000];
	char s1[1000];
	char s2[1000];
	for (i=0; i<t; i++)
	{
		scanf("%s",&s);
		int j=0,l1=0,l2=0;
		while (s[j]>='0'&&s[j]<='9')
			s1[l1++]=s[j++];
		s1[l1]=0;
		c=s[j++];
		while (s[j])
			s2[l2++]=s[j++];
		s2[l2]=0;
		bignum n1(1000,s1);
		bignum n2(1000,s2);
		if (c-'*')
		{
			bignum n3(1000,n1);
			if (c=='+')
				n3+=n2;
			else
				n3-=n2;
			int line1=max(n2.length+1,n3.length);
			int maxlen=max(n1.length,line1);
			for (j=0; j<maxlen-n1.length; j++)
				putchar(' ');
			for (j=n1.length-1; j>=0; j--)
				putchar(n1.digits[j]+'0');
			putchar('\n');
			for (j=0; j<maxlen-n2.length-1; j++)
				putchar(' ');
			putchar(c);
			for (j=n2.length-1; j>=0; j--)
				putchar(n2.digits[j]+'0');
			putchar('\n');
			for (j=0; j<maxlen-line1; j++)
				putchar(' ');
			for (j=0; j<line1; j++)
				putchar('-');
			putchar('\n');
			for (j=0; j<maxlen-n3.length; j++)
				putchar(' ');
			for (j=n3.length-1; j>=0; j--)
				putchar(n3.digits[j]+'0');
		}
		else
		{
			//more difficult
			bignum prod(1000,n1);
			prod*=n2;
			bignum* multiples=new bignum[10];
			bignum total(1000,0);
			for (j=0; j<10; j++)
			{
				multiples[j]=total;
				total+=n1;
			}
			int maxlen=max(prod.length,n2.length+1);
			for (j=0; j<maxlen-n1.length; j++)
				putchar(' ');
			for (j=n1.length-1; j>=0; j--)
				putchar(n1.digits[j]+'0');
			putchar('\n');
			for (j=0; j<maxlen-n2.length-1; j++)
				putchar(' ');
			putchar('*');
			for (j=n2.length-1; j>=0; j--)
				putchar(n2.digits[j]+'0');
			putchar('\n');
			if (n2.length==1)
			{
				int line1=max(n2.length+1,prod.length);
				for (j=0; j<maxlen-line1; j++)
					putchar(' ');
				for (j=0; j<line1; j++)
					putchar('-');
				putchar('\n');
				for (j=0; j<maxlen-prod.length; j++)
					putchar(' ');
				for (j=prod.length-1; j>=0; j--)
					putchar(prod.digits[j]+'0');
			}
			else
			{
				int line1=max(n2.length+1,multiples[n2.digits[0]].length);
				for (j=0; j<maxlen-line1; j++)
					putchar(' ');
				for (j=0; j<line1; j++)
					putchar('-');
				putchar('\n');
				for (j=0; j<n2.length; j++)
				{
					for (k=0; k<maxlen-multiples[n2.digits[j]].length-j; k++)
						putchar(' ');
					for (k=multiples[n2.digits[j]].length-1; k>=0; k--)
						putchar(multiples[n2.digits[j]].digits[k]+'0');
					for (k=0; k<j; k++)
						putchar(' ');
					putchar('\n');
				}
				for (j=0; j<maxlen-prod.length; j++)
					putchar(' ');
				for (j=0; j<prod.length; j++)
					putchar('-');
				putchar('\n');
				for (j=0; j<maxlen-prod.length; j++)
					putchar(' ');
				for (j=prod.length-1; j>=0; j--)
					putchar(prod.digits[j]+'0');
			}
		}
		putchar('\n');
		putchar('\n');
	}
	return 0;
}
