// 2008-05-28
#include <cstdio>
#include <cstring>

#define MAXN	50000
#define M	98765431

int res[2],v[2][2],tem[2][2],t,t1;
int lis[MAXN],dif[MAXN],n;
long long tot;

void mul(int a[2][2],int b[2][2]){
	int i,j,k;
	for(i=0;i<2;i++)
		for(j=0;j<2;j++)
			for(tem[i][j]=0,k=0;k<2;k++)
				tem[i][j]=(tem[i][j]+(long long)a[i][k]*b[k][j])%M;
		
}

int main(){
	int i;
	scanf("%d%d",&n,&t);
	for(i=tot=0,0;i<n;i++){
		scanf("%d",&lis[i]);
		dif[i]=lis[i]-lis[0];
		tot+=dif[i];
	}
	memset(v,0,sizeof(v));
	memset(res,0,sizeof(res));
	res[0]=lis[0]%M;
	res[1]=tot%M;
	v[0][0]=(n-1)%M;
	v[0][1]=1;
	v[1][1]=-1;
	for(t1=t;t1;t1/=2){
		if(t1%2){
			res[0]=((long long)res[0]*v[0][0]+(long long)res[1]*v[0][1])%M;
			res[1]=((long long)res[1]*v[1][1])%M;
		}
		mul(v,v);
		memcpy(v,tem,sizeof(v));
	}
	for(i=0;i<n;i++)
		printf("%d\n",((res[0]+((t%2==0)?(dif[i]):(-dif[i])))%M+M)%M);
	return 0;
}
