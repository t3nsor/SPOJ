// 2010-11-15
#include <cstdio>
#include <cstring>
#include <iostream>
//personal bignum library
typedef unsigned int uint;
struct bignum
{
	uint max(uint x,uint y)
	{
		return x>y?x:y;
	}
	uint min(uint x,uint y)
	{
		return x<y?x:y;
	}
	uint* digits;
	uint size;
	uint length;
	bignum(uint s=400)
	{
		length=1;
		digits=new uint[size=s];
		digits[0]=0;
	}
	bignum(const bignum& x)     //Copy constructor
	{
		digits=new uint[size=x.size];
		length=x.length;
		memcpy(digits,x.digits,x.length*sizeof(uint));
	}
	bignum(uint s,const bignum& x)
	{
		if (s<x.length) throw;
		length=x.length;
		digits=new uint[size=s];
		memcpy(digits,x.digits,sizeof(uint)*x.length);
	}
	bignum(const char* x)
	{
		int slen=strlen(x),i;
		digits=new uint[size=(slen+8)/9];
		for (length=0; slen>0; length++,slen-=9)
		{
			digits[length]=0;
			for (i=slen>9?slen-9:0; i<slen; i++)
				digits[length]=10*digits[length]+x[i]-'0';
		}
		length=size;
	}
	bignum(uint s,const char* x)
	{
		digits=new uint[size=s];
		int slen=strlen(x),i;
		for (length=0; slen>0; length++,slen-=9)
		{
			digits[length]=0;
			for (i=slen>9?slen-9:0; i<slen; i++)
				digits[length]=10*digits[length]+x[i]-'0';
		}
	}
	void operator=(const bignum& y)
	{
		length=y.length;
		memcpy(digits,y.digits,sizeof(uint)*y.length);
	}
	void operator=(unsigned long long x)
	{
		digits[1]=x/1000000000;
		digits[0]=x%1000000000;
		length=1+!!digits[1];
	}
	void operator=(const char* s)
	{
		int slen=strlen(s),i,l;
		for (l=0; slen>0; l++,slen-=9)
		{
			digits[l]=0;
			for (i=slen>9?slen-9:0; i<slen; i++)
				digits[l]=10*digits[l]+s[i]-'0';
		}
		length=l;
	}
	~bignum()
	{
		delete[] digits;
	}
	void add(bignum& x)
	{
		int l=max(length,x.length);
		int d;
		int carry=0,sum;
		for (d=0; d<=l; d++)
		{
			sum=carry;
			if (d<length)
				sum+=digits[d];
			if (d<x.length)
				sum+=x.digits[d];
			digits[d]=sum;
			if (digits[d]>=1000000000) digits[d]-=1000000000,carry=1; else carry=0;
		}
		length=l;
		if (digits[length]) length++;
	}
	void sub(bignum& x)
	{
		if ((*this)<x)
			throw; //negative numbers not yet supported
		int d;
		int borrow=0;
		for (d=0; d<=length; d++)
		{
			digits[d]-=borrow;
			if (d<x.length)
				digits[d]-=x.digits[d];
			if (digits[d]>>31) {digits[d]+=1000000000;borrow=1;} else borrow=0;
		}
		while (length>0&&!digits[length-1]) length--;
		if (length==0) length=1;
	}
	void shl()
	{
		memmove(digits+1,digits,sizeof(uint)*length);
		digits[++length]=0;
	}
	void mult(bignum& x)
	{
		bignum res(length+x.length);
		res.length=length+x.length;
		memset(res.digits,0,res.size*sizeof(uint));
		unsigned long long y,z;
		int i,j,k;
		for (i=0; i<length; i++)
			for (j=0; j<x.length; j++)
			{
				y=digits[i];
				y*=x.digits[j];
				res.digits[i+j+1]+=z=y/1000000000;
				res.digits[i+j]+=y-1000000000*z; //mod is slow
				k=i+j;
				while (k<res.length&&res.digits[k]>=1000000000)
				{
					res.digits[k]-=1000000000;
					res.digits[++k]++;
				}
			}
		while (res.length&&!res.digits[res.length-1])
			res.length--;
		*this=res;
	}
	void print()
	{
		int i;
		for (i=length-1; i+1&&!digits[i]; i--);
		if (i<0) i=0;
		printf("%u",digits[i--]);
		while (i+1)
			printf("%.9u",digits[i--]);
	}
	void println()
	{
		print();
		puts("");
	}
	bignum operator+(bignum& y)
	{
		bignum res(max(length,y.length)+1,*this);
		res.add(y);
		return res;
	}
	bignum operator-(bignum& y)
	{
		bignum res(max(length,y.length),*this);
		res.sub(y);
		return res;
	}
	bignum operator*(bignum& y)
	{
		bignum res(length+y.length,*this);
		res.mult(y);
		return res;
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
	bool operator==(const bignum& y)
	{
		int i;
		if (length!=y.length)
			return false;
		for (i=length-1; i>=0; i--)
			if (digits[i]!=y.digits[i])
				return false;
		return true;
	}
	bool operator<(const bignum& y)
	{
		int i;
		if (length<y.length)
			return true;
		if (length>y.length)
			return false;
		for (i=length-1; i>=0; i--)
			if (digits[i]<y.digits[i])
				return true;
			else if (digits[i]>y.digits[i])
				return false;
		return false;
	}
	bool operator>(const bignum& y)
	{
		int i;
		if (length>y.length)
			return true;
		if (length<y.length)
			return false;
		for (i=length-1; i>=0; i--)
			if (digits[i]>y.digits[i])
				return true;
			else if (digits[i]<y.digits[i])
				return false;
		return false;
	}
	bool operator<=(const bignum& y)
	{
		return !operator>(y);
	}
	bool operator>=(const bignum& y)
	{
		return !operator<(y);
	}
};
int main()
{
	bignum two[1000];
	bignum answers[1000];
	bignum lol(1); lol=2;
	bignum lol2(10);
	int i;
	two[0]=1;
	for (i=1; i<1000; i++)
	{
		two[i]=two[i-1]; two[i]*=lol;
		answers[i]=two[i];
		if (2-4*(i%2)>=0)
		{
			lol2=2-4*(i%2);
			answers[i]+=lol2;
		}
		else
		{
			lol2=4*(i%2)-2;
			answers[i]-=lol2;
		}
		//manipulate answers: nasty
		int j=answers[i].length-1;
		long long z=0;
		while (j>=0)
		{
			int t=answers[i].digits[j];
			answers[i].digits[j]=(z+answers[i].digits[j])/6;
			z=1000000000*((z+t)%6);
			j--;
		}
		if (answers[i].length>1&&answers[i].digits[answers[i].length-1]==0)
			answers[i].length--;
//		answers[i].println();
	}
	int x;
	while (scanf("%d",&x)==1)
		answers[x].println();
	return 0;
}

