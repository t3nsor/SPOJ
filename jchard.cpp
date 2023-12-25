// 2008-07-17
// updated 2023-12-25 for legibility
// The output is the text file to submit.
// No, I don't remember how this works :)
#include <string.h>
#include <iostream>
using namespace std;
int main() {
    bool a[100][100];
    int i,j;
    memset(a,false,sizeof(a));
    for (i=0; i<50; i++) {
        for (j=0; j<50; j++) {
            a[i][j]=true;
        }
    }
    for (i=50; i<99; i++) {
        a[i][i-1]=true;
        a[i][i+1]=true;
        a[i-1][i]=true;
        a[i+1][i]=true;
    }
    a[49][50]=true;
    a[99][98]=true;
    puts("50");
    for (i=0; i<100; i++) {
        for (j=0; j<100; j++) {
            printf("%d ",a[i][j]?1:0);
        }
        putchar('\n');
    }
    return 0;
}
