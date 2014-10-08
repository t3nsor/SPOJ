# 2008-04-16
import sys
import random
import math
ACCURACY=int(50)
def bits(integer):
	result = 0
	while integer:
		integer >>= 1
		result += 1
	return result
def power(base, exponent, modulo):
	result = 1L
	firstModulus = base % modulo
	iteration = bits(exponent)
	while iteration >= 0:
		result = (result * result) % modulo
		if (exponent >> iteration) & 1:
			result = (result * firstModulus) % modulo
		iteration -= 1
	return result
def RabinMiller(n):
	if n<2:
		return 0
	if n==2:
		return 1
	if n&1==0:
		return 0
	s=0
	_2s=1
	nm1=n-1
	while nm1&1==0:
		s=s+1
		_2s=_2s<<1
		nm1=nm1>>1
	d=nm1
	for i in range(ACCURACY):
		a=random.randint(1,n-1)
		b=1
		for j in range(s):
			if power(a,d,n)==1 or power(a,(1<<j)*d,n)==n-1:
				b=0
				break
		if b:
			return 0
	return 1
random.seed()
t=int(sys.stdin.readline())
for i in range(t):
	N=int(sys.stdin.readline())
	if RabinMiller(N):
		print "YES"
	else:
		print "NO"
