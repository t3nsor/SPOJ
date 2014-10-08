/* 2010-11-09
 * This is based on "Simple Linear Work Suffix Array Construction", but the
 * radix sort is hideously inefficient. TODO: Rewrite this
 * Also featured is Kasai's algorithm for computing lcp array.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void* qsort_global_pointer;
int qsort_global_size;
#define TYPE int
#define FALSE 0
#define TRUE 1
int comparator(const void* a,const void* b)
{
	const TYPE* s=(const TYPE*)qsort_global_pointer;
	int i=*(const int*)a;
	int j=*(const int*)b;
	int N = qsort_global_size;
	while (i<N&&j<N)
		if (s[i]<s[j]) return -1;
		else if (s[i]>s[j]) return 1;
		else {++i; ++j;}
	if (i==N)
		if (j==N)
			return 0;
		else
			return -1;
	else
		return 1;
}
const int LOG_RADIX_PASS_BITS=3;
const int RADIX_PASS_BITS=8;
void radix_sortc(TYPE* s,int N,int sz,TYPE* res)
{
	int i,j,k,m,p;
	for (i=0; i<N; ++i)
		res[i]=i;
	//--system of linked lists--
	int first[1<<RADIX_PASS_BITS];
	int last[1<<RADIX_PASS_BITS];
	int cnt;
	int data[N];
	int next[N];
	//--------------------------
	for (k=1; k<=sz*8/RADIX_PASS_BITS; ++k)
	{
		for (p=0; p<sizeof(TYPE)*8/RADIX_PASS_BITS; ++p)
		{
			memset(first,-1,sizeof(first));
			memset(last,-1,sizeof(last));
			memset(next,-1,sizeof(next));
			cnt=0;
			for (i=0; i<N; ++i)
			{
				int bucket=(s[sz*(res[i]+1)-k/(8/RADIX_PASS_BITS)]>>
					(p<<LOG_RADIX_PASS_BITS))&((1<<RADIX_PASS_BITS)-1);
				data[cnt]=res[i];
				if (~last[bucket])
				{
					next[last[bucket]]=cnt;
					last[bucket]=cnt++;
				}
				else
					first[bucket]=last[bucket]=cnt++;
			}
			for (i=0, m=0; i<(1<<RADIX_PASS_BITS); ++i)
				for (j=first[i]; ~j; j=next[j])
					res[m++]=data[j];
		}
	}
//	return res;
}
const int CUTOFF = 20;
void suffix_array(TYPE* s,int size,TYPE* res)
{
	int i,j,k;
	if (size<CUTOFF) //manual
	{
		for (i=0; i<size; ++i)
			res[i]=i;
		qsort_global_pointer=(void*)s;
		qsort_global_size=size;
		qsort(res,size,sizeof(TYPE),comparator);
	}
	else
	{
		//zero-pad the string
		s[size]=0;
		s[size+1]=0;
		int N=size+2;
		int pieces_size=3*((N-1)/3)+3*((N-2)/3);
		TYPE pieces[pieces_size];
		//step 1
		memcpy(pieces,s+1,3*((N-1)/3)*sizeof(TYPE));
		memcpy(pieces+3*((N-1)/3),s+2,3*((N-2)/3)*sizeof(TYPE));
		TYPE sa[pieces_size/3]; radix_sortc(pieces,pieces_size/3,3,sa);
		TYPE rank[pieces_size/3+3]; //extra space for recursive call
		//identical pieces must receive identical lexnames
		rank[sa[0]]=0;
		int flag=FALSE;
		for (i=1; i<pieces_size/3; ++i)
			if (
				pieces[3*sa[i-1]]==pieces[3*sa[i]]&&
				pieces[3*sa[i-1]+1]==pieces[3*sa[i]+1]&&
				pieces[3*sa[i-1]+2]==pieces[3*sa[i]+2]
				)
			{
				flag=TRUE;
				rank[sa[i]]=rank[sa[i-1]];
			}
			else
				rank[sa[i]]=rank[sa[i-1]]+1;
		if (flag) //recursive call
		{
			suffix_array(rank,pieces_size/3,sa);
			for (i=0; i<pieces_size/3; ++i)
				rank[sa[i]]=i;
		}
		// step 2
		TYPE v[2*(N/3)];
		for (i=0; i<N/3; ++i)
		{
			v[2*i]=s[3*i];
			v[2*i+1]=rank[i];
		}
		TYPE sa2[N/3]; radix_sortc(v,N/3,2,sa2);
		// step 3
		i=0;
		j=0;
		k=0;
		while (i<pieces_size/3 && j<N/3)
		{
			int less;
			if (sa[i]<(N-1)/3) //[1]
				less =
					s[3*sa[i]+1]<s[3*sa2[j]] ||
					s[3*sa[i]+1]==s[3*sa2[j]] &&
						rank[sa[i]+(N-1)/3]<rank[sa2[j]];
			else //[2]
				less =
					s[3*(sa[i]-(N-1)/3)+2]<s[3*sa2[j]] ||
					s[3*(sa[i]-(N-1)/3)+2]==s[3*sa2[j]] &&
						s[3*(sa[i]-(N-1)/3)+3]<s[3*sa2[j]+1] ||
					s[3*(sa[i]-(N-1)/3)+2]==s[3*sa2[j]] &&
						s[3*(sa[i]-(N-1)/3)+3]==s[3*sa2[j]+1] &&
						rank[sa[i]-(N-1)/3+1]<rank[sa2[j]+(N-1)/3];
			if (less)
			{
				if (sa[i]<(N-1)/3)
					res[k++]=3*sa[i]+1;
				else
					res[k++]=3*(sa[i]-(N-1)/3)+2;
				++i;
			}
			else
				res[k++]=3*sa2[j++];
		}
		if (i==pieces_size/3)
			while (j<N/3)
				res[k++]=3*sa2[j++];
		if (j==N/3)
			while (i<pieces_size/3)
			{
				if (sa[i]<(N-1)/3)
					res[k++]=3*sa[i]+1;
				else
					res[k++]=3*(sa[i]-(N-1)/3)+2;
				++i;
			}
//		return res;
	}
}
int main()
{
	int N;
	scanf("%d\n",&N);
	static char input[222222];
	gets(input);
	int i,j,k;
	TYPE V[N+3];
	for (i=0; i<N; ++i)
		V[i]=input[i];
	V[N]=0; //unique terminator
	TYPE sa[N+1]; suffix_array(V,N+1,sa);
	int rank[N+1];
	for (i=0; i<=N; ++i)
		rank[sa[i]]=i;
	int res=0;
	k=0;
	for (i=0; i<=N; ++i)
	{
		if (k>0) --k;
		if (rank[i]==N)
		{
			k=0;
			continue;
		}
		j=sa[rank[i]+1];
		while (i+k<=N&&j+k<=N&&V[i+k]==V[j+k]) ++k;
		if (k>res) res=k;
	}
	printf("%d\n",res);
	return 0;
}

