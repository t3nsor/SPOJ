/* 2009-01-04 */
int main()
{
	char c1,c2;
	printf("Ready\n");
	for(;;)
	{
		c1=getchar(); c2=getchar(); getchar();
		if (c1==' '&&c2==' ')
			return 0;
		if (c1=='b'&&c2=='d'||c1=='d'&&c2=='b'||c1=='p'&&c2=='q'||c1=='q'&&c2=='p')
			printf("Mirrored pair\n");
		else
			printf("Ordinary pair\n");
	}
}
