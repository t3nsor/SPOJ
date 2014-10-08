// 2010-11-13
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
using namespace std;
typedef long double LD;
const LD pi=3.14159265358979323846;
int main()
{
	int N,cnt=0,i,j;
	pair<LD,LD> P[1500];
	LD A[1500],ang;
	scanf("%d",&N);
	for (i=0; i<N; i++)
		scanf("%Lf %Lf",&P[i].first,&P[i].second);
	for (i=0; i<N; i++)
	{
		for (j=0; j<N; j++)
		{
			ang=atan2(P[j].second-P[i].second,P[j].first-P[i].first);
			if (ang<0) ang+=pi;		
			if (j<i) A[j]=ang; else if (j>i) A[j-1]=ang;
		}
		sort(A,A+N-1);
		for (j=0; j<N-1&&A[j]<pi/2+1e-15; j++)
			cnt+=upper_bound(A,A+N-1,A[j]+pi/2+1e-15)-lower_bound(A,A+N-1,A[j]+pi/2-1e-15);
	}
	printf("%d\n",cnt);
	return 0;
}
