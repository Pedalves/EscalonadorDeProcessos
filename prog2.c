#include <stdio.h>
#include <unistd.h>

int main()
{
	int i;

	for(i=0; i<10; i++)
	{
		printf("prog2\n");
		sleep(1);
	}

	return 0;
}