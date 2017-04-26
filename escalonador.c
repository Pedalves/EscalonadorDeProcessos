#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "heap.h"

#define MAX 30 

void addPrioridade(char* prog, int prioridade)
{
	/*char* vet3[] = {prog, NULL};	
	pid_t pid;

	if ((pid = fork()) < 0) 
	{ 
		fprintf(stderr, "Erro ao criar filho\n"); 
		exit(-1); 
	} 
	if (pid1 == 0)
	{
		execv("prog1", vet1);
		exit(1);
	}
	kill(pid, SIGSTOP);*/
}

int main()
{
	int PRIORIDADE = 0, ROUND-ROBIN = 0, REAL-TIME = 0;

	Heap* heapPrioridade = heap_cria(MAX);

	while(true)
	{
		if(REAL-TIME)
		{

		}
		else if(PRIORIDADE)
		{
			//termina o processo corrente ou continua lendo do interpretador?
		}
		else if(ROUND-ROBIN)
		{

		}
	}	
	
	return 0;
}
