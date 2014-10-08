# 2009-04-14
def factorial(x):
	result = 1
	while x:
		result *= x
		x -= 1
	return result
while 1:
	n = int(raw_input())
	if n == -1:
		break;
	print factorial(2*n)/(factorial(n)*factorial(n+1))
