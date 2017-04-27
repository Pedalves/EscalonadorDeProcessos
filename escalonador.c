#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "heap.h"

#define MAX 30 

pid_t addPrioridade(char* prog, int prioridade)
{
	char* vet3[] = {prog, NULL};	
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
	kill(pid, SIGSTOP);

	return pid;
}

int main()
{
	int PRIORIDADE = 0, ROUND-ROBIN = 0, REAL-TIME = 0;	

	// REAL-TIME
	pid_t pidPrioridade = -1
	
	// PRIORIDADE
	Heap* heapPrioridade = heap_cria(MAX);
	pid_t pidPrioridade = -1;	
	
	// ROUND-ROBIN
	pid_t roundRobin[MAX]; 
	int numberRR = 0, currRR = 0;

	int i;

	for(i = 0; i<MAX; i++)
		roudRobin[i] = -1;	

	while(true)
	{
		if(REAL-TIME)
		{

		}
		else if(PRIORIDADE)
		{
			heap_remove(heapPrioridade);
		}
		else if(ROUND-ROBIN)
		{
			kill(roundRobin[currRR], SIGCONT);
			sleep(0.5);
			kill(roundRobin[currRR], SIGSTOP);

			if(currRR < numberRR)
				currRR++;
			else
				currRR = 0;
		}
	}	

	return 0;
}
