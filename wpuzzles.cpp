// 2008-09-28
// modified 2014-10-07
#include <iostream>
#include <cstring>
#include <ext/hash_map>
using namespace std;
using namespace __gnu_cxx;
pair<int,int> found[1000];
char word[1000][1001];
int len[1000];
int row[1000];
int col[1000];
int dir[1000];
typedef int hashtype;
const hashtype RADIX=15485867;
const hashtype RADIX_INVERSE=3935065859;
hashtype pwr;
hash_map<hashtype,int> hashes;
int N;
bool equ;
hashtype f(int x)
{
    hashtype result=1;
    hashtype cur=RADIX;
    while (x)
    {
        if (x&1)
            result*=cur;
        x>>=1;
        cur*=cur;
    }
    return result;
}
int RabinKarp(char* haystack)
{
    hashtype cur=1;
    hashtype h2=0;
    int i;
    for (i=0; i<len[0]; i++)
    {
        if (!haystack[i])
            return 0;
        h2+=cur*haystack[i];
        cur*=RADIX;
    }
    int cnt=0;
    int pos;
    for(pos=0;;pos++)
    {
        hash_map<hashtype,int>::iterator It=hashes.find(h2);
        if (It!=hashes.end())
        {
            bool ok=true;
            for (i=0; i<len[0]; i++)
                if (word[It->second][i]-haystack[pos+i])
                {
                    ok=false;
                    break;
                }
            if (ok)
                found[cnt++]=make_pair(It->second,pos);
        }
        if (!haystack[pos+len[0]])
            break;
        h2=(h2-haystack[pos])*RADIX_INVERSE+pwr*haystack[pos+len[0]];
    }
    return cnt;
}
int NaiveSearch(char* haystack)
{
    int cnt=0;
    int i;
    for (i=0; i<N; i++)
    {
        if (row[i]+1)
            continue;
        char* p=strstr(haystack,word[i]);
        if (p)
            found[cnt++]=make_pair(i,int(p-haystack));
    }
    return cnt;
}
int search(char* haystack)
{
    if (equ)
        return RabinKarp(haystack);
    else
        return NaiveSearch(haystack);
}
int main()
{
    int T;
    scanf("%d",&T);
    int i,j,k,m,X,Y;
    static char grid[1001][1001];
    char tmp[2100];
    char tmp2[2100];
    int cnt;
    while (T--)
    {
        scanf("%d %d %d",&Y,&X,&N);
        getchar();
        for (i=0; i<Y; i++)
            gets(grid[i]);
        equ=true;
        for (i=0; i<N; i++)
        {
            gets(word[i]);
            len[i]=strlen(word[i]);
            if (len[i]-len[0])
                equ=false;
            row[i]=-1;
        }
        hashes.clear();
        if (equ)
        {
            pwr=f(len[0]-1);
            for (i=0; i<N; i++)
            {
                hashtype h=word[i][len[0]-1];
                for (j=len[0]-2; j>=0; j--)
                    h=RADIX*h+word[i][j];
                if (hashes.find(h)!=hashes.end())
                    throw;
                hashes[h]=i;
            }
        }
        //search C and G
        for (i=0; i<Y; i++)
        {
            strcpy(tmp,grid[i]);
            int beg=0;
            int end=X-1;
            while (beg<end)
                swap(tmp[beg++],tmp[end--]);
            int n=search(grid[i]);
            for (j=0; j<n; j++)
            {
                row[found[j].first]=i;
                col[found[j].first]=found[j].second;
                dir[found[j].first]='C';
            }
            n=search(tmp);
            for (j=0; j<n; j++)
            {
                row[found[j].first]=i;
                col[found[j].first]=X-found[j].second-1;
                dir[found[j].first]='G';
            }
        }
        //search A and E
        for (i=0; i<X; i++)
        {
            for (j=0; j<Y; j++)
            {
                tmp[j]=grid[j][i];
                tmp2[j]=grid[Y-j-1][i];
            }
            tmp[Y]=0;
            tmp2[Y]=0;
            int n=search(tmp);
            for (j=0; j<n; j++)
            {
                row[found[j].first]=found[j].second;
                col[found[j].first]=i;
                dir[found[j].first]='E';
            }
            n=search(tmp2);
            for (j=0; j<n; j++)
            {
                row[found[j].first]=Y-found[j].second-1;
                col[found[j].first]=i;
                dir[found[j].first]='A';
            }
        }
        //search D and H
        for (i=Y-1; i>=1-X; i--)
        {
            int r=max(i,0);
            int c=max(0,-i);
            for (j=r,k=c,cnt=0; j<Y&&k<X; j++,k++,cnt++)
                tmp[cnt]=grid[j][k];
            tmp[cnt]=0;
            strcpy(tmp2,tmp);
            j=0;
            k=cnt-1;
            while (j<k)
                swap(tmp2[j++],tmp2[k--]);
            int n=search(tmp);
            for (j=0; j<n; j++)
            {
                row[found[j].first]=r+found[j].second;
                col[found[j].first]=c+found[j].second;
                dir[found[j].first]='D';
            }
            n=search(tmp2);
            for (j=0; j<n; j++)
            {
                row[found[j].first]=r+cnt-found[j].second-1;
                col[found[j].first]=c+cnt-found[j].second-1;
                dir[found[j].first]='H';
            }
        }
        //search B and F, finally
        for (i=0; i<=X+Y-2; i++)
        {
            int r=min(X-1,i);
            int c=max(0,i-Y+1);
            for (j=r,k=c,cnt=0; j>=0&&k<X; j--,k++,cnt++)
                tmp[cnt]=grid[j][k];
            tmp[cnt]=0;
            strcpy(tmp2,tmp);
            j=0;
            k=cnt-1;
            while (j<k)
                swap(tmp2[j++],tmp2[k--]);
            int n=search(tmp);
            for (j=0; j<n; j++)
            {
                row[found[j].first]=r-found[j].second;
                col[found[j].first]=c+found[j].second;
                dir[found[j].first]='B';
            }
            n=search(tmp2);
            for (j=0; j<n; j++)
            {
                row[found[j].first]=r-(cnt-found[j].second-1);
                col[found[j].first]=c+(cnt-found[j].second-1);
                dir[found[j].first]='F';
            }
        }
        for (i=0; i<N; i++)
        {
            printf("%d %d %c\n",row[i],col[i],dir[i]);
        }
        if (T) putchar('\n');
    }
    return 0;
}
