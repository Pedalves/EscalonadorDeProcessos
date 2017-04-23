#include <stdio.h>
#include <unistd.h>

int main()
{
	int i;
	int fatorial = 1;

	for(i=5; i>0; i--)
	{
		printf("itercao %d %d\n", 5-i, fatorial);
		fflush(stdout);
		fatorial *= i;
		sleep(0.5);
	}

	return 0;
}