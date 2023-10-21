// 2020-10-20
#include <sstream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
using namespace std;
typedef unsigned long long LL;
#define next1(x) ((x<(N>>1))?(x<<1):((x<<1)-N+1))
#define next2(x) ((x&1)?(x>>1)+(N>>1):(x>>1))
int GCD(int x,int y) {
    if (x<y)
        return GCD(y,x);
    if (y==0)
        return x;
    return GCD(y,x%y);
}
int LCM(int x,int y) {
    LL res=(LL)x*(LL)y/GCD(x,y);
    if (res>2147483647) throw;
    return res;
}
static int a[1024][1024];
static int A[1024][1024];
int tmp[1024];
int tmp2[1024];
static bool b[1024][1024];
bool B[1024];
int main() {
    for(;;) {
        int N,i,j,cnt,z;
        scanf("%d",&N);
        getchar();
        if (!N) return 0;
        for (i=0; i<N; i++) {
            for (j=0; j<N; j++) {
                a[i][j]=j+(i<<10);
            }
        }
        char s[1000];
        fgets(s, 1000, stdin);
        istringstream ss; ss.str(s);
        vector<string> cmds;
        string cmd;
        while (ss >> cmd) {
            cmds.push_back(cmd);
        }
        for (int k = cmds.size() - 1; k >= 0; k--) {
            cmd=cmds[k];
            if (cmd[0]=='i') //id or id-
                continue;
            if (cmd[0]=='r') {
                if (cmd.length()==3) {
                    //90 degree rotation counterclockwise
                    for (i=0; i<(N>>1); i++)
                        for (j=0; j<(N>>1); j++) {
                            z=a[j][i];
                            a[j][i]=a[i][N-j-1];
                            a[i][N-j-1]=a[N-j-1][N-i-1];
                            a[N-j-1][N-i-1]=a[N-i-1][j];
                            a[N-i-1][j]=z;
                        }
                } else {
                    //90 degree rotation clockwise
                    for (i=0; i<(N>>1); i++)
                        for (j=0; j<(N>>1); j++) {
                            z=a[j][i];
                            a[j][i]=a[N-i-1][j];
                            a[N-i-1][j]=a[N-j-1][N-i-1];
                            a[N-j-1][N-i-1]=a[i][N-j-1];
                            a[i][N-j-1]=z;
                        }
                }
            } else if (cmd[0]=='s') {
                //horizontal symmetry
                for (j=0; j<N; j++)
                    for (i=0; i<(N>>1); i++)
                        swap(a[j][i],a[j][N-i-1]);
            } else if (cmd[1]=='h') {
                //bhsym+-
                for (j=N>>1; j<N; j++)
                    for (i=0; i<(N>>1); i++)
                        swap(a[j][i],a[j][N-i-1]);
            } else if (cmd[1]=='v') {
                //bvsym+-
                for (j=(N>>1),z=N-1; j<z; j++,z--)
                {
                    memcpy(tmp,a[j],N<<2);
                    memcpy(a[j],a[z],N<<2);
                    memcpy(a[z],tmp,N<<2);
                }
            }
            else if (cmd[0]=='d') {
                //div
                memset(B,false,sizeof(bool)*N);
                if (cmd.length()==3) {
                    //div+
                    for (i=0; i<N; i++) {
                        if (B[i]) continue;
                        B[i]=true;
                        memcpy(tmp,a[i],N<<2);
                        j=i;
                        while ((z=next1(j))!=i) {
                            B[z]=true;
                            memcpy(a[j],a[z],N<<2);
                            j=z;
                        }
                        memcpy(a[j],tmp,N<<2);
                    }
                } else {
                    //div-
                    for (i=0; i<N; i++) {
                        if (B[i]) continue;
                        B[i]=true;
                        memcpy(tmp,a[i],N<<2);
                        j=i;
                        while ((z=next2(j))!=i) {
                            B[z]=true;
                            memcpy(a[j],a[z],N<<2);
                            j=z;
                        }
                        memcpy(a[j],tmp,N<<2);
                    }
                }
            } else if (cmd.length()==3) {
                //mix+
                for (i=0; i<N; i+=2) {
                    memcpy(tmp,a[i],N<<2);
                    memcpy(tmp2,a[i+1],N<<2);
                    for (j=0; j<N; j+=2) {
                        a[i][j]=tmp[j>>1];
                        a[i][j+1]=tmp2[j>>1];
                        a[i+1][j]=tmp[(j>>1)+(N>>1)];
                        a[i+1][j+1]=tmp2[(j>>1)+(N>>1)];
                    }
                }
            } else {
                //mix-
                for (i=0; i<N; i+=2) {
                    memcpy(tmp,a[i],N<<2);
                    memcpy(tmp2,a[i+1],N<<2);
                    for (j=0; j<N; j+=2) {
                        a[i][j>>1]=tmp[j];
                        a[i][(j>>1)+(N>>1)]=tmp2[j];
                        a[i+1][j>>1]=tmp[j+1];
                        a[i+1][(j>>1)+(N>>1)]=tmp2[j+1];
                    }
                }
            }
        }
        /*for (i=0; i<N; i++)
          memset(b[i],false,sizeof(bool)*N);*/
        memset(b,false,sizeof(b));
        int x,y,t;
        LL res=1;
        for (j=0; j<N; j++) {
            for (i=0; i<N; i++) {
                if (b[j][i])
                    continue;
                x=i,y=j;
                cnt=0;
                while (!b[y][x]) {
                    b[y][x]=true;
                    cnt++;
                    t=x;
                    x=a[y][x]&1023;
                    y=a[y][t]>>10;
                }
                res=LCM(res,cnt);
            }
        }
        printf("%lld\n",res);
    }
}
