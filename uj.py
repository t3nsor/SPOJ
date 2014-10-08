# 2008-06-24
import sys
while 1:
        s=raw_input()
        A=0
        B=0
        D=0
        j=0
        while (s[j]!=' '):
                j+=1
        k=0
        while (k<j):
                A=10*A+int(s[k])
                k+=1
        k=j+1
        while (k<len(s)):
                B=10*B+int(s[k])
                k+=1
        if (A==0):
                exit()
        else:
                print A**B
