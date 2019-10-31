#include <bits/stdc++.h>
using namespace std;


int main () {
  int n;
  int arr1[100001];
  int arr2[100001];
  int index1, index2, bottom2, nbArr1, nbArr2;
  index1 = -1; index2 = -1; bottom2 = 0; nbArr1 = 0; nbArr2 = 0;
  while (scanf ("%d",&n) != EOF) {
    if (n == 0) {
      printf("\n");
      index1 = -1; index2 = -1; bottom2 = 0; nbArr1 = 0; nbArr2 = 0;
    }
    else if (n == -1) {
      printf("%d\n", arr1[index1]);
      index1 = index1 - 1;
      nbArr1 --;
      if (nbArr1 < nbArr2) {
        nbArr1 ++;
        nbArr2 --;
        index1++;
        arr1[index1] = arr2[bottom2];
        bottom2 ++;
      }
    }
    else {
      if (nbArr1 == 0 && nbArr2 == 0) {
        nbArr1 ++;
        index1 ++;
        arr1[index1] = n;
      }
      else if (nbArr1 > nbArr2) {
        nbArr2 ++;
        index2 ++;
        arr2[index2] = n;
      }
      else {
        nbArr1 ++;
        index1 ++;
        arr1[index1] = arr2[bottom2];
        bottom2 ++;
        index2 ++;
        arr2[index2] = n;
      }
    }
  }
}