# 2008-07-14
N=int(raw_input())
for i in range(N):
	n,k=([long(x) for x in raw_input().split()])
	n-=1
	k-=1
	if k>n/2:
		k=n-k
	num=1
	den=1
	for j in range(k):
		num*=n-j
		den*=j+1
	print num/den
		
