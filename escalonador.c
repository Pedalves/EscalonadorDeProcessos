#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#include "heap.h"

#define MAX 30 

int PRIORIDADE = 0, ROUND_ROBIN = 0, REAL_TIME = 0;	

int shmProcesso; // segmento (shm) para disparo de novos processos no escalonador

void adicionarProcesso(int signal) 
{
	char *novoProcesso; // Nome do novo processo (shm)
	char nome[20];

	int i, len, prioridade;
	char msgCriacao[200];

	novoProcesso = (char *) shmat(shmProcesso, 0, 0);

	sscanf(novoProcesso, "%s", nome);

	printf("->%s\n", nome);
	fflush(stdout);

	len = strlen(novoProcesso);

	if(len > 3 && novoProcesso[len-2] == '=' && novoProcesso[len-3] == 'R') // Prioridade
	{
		//addPrioridade(nome, novoProcesso[len-1])
	}
	else if(len > 3 && (novoProcesso[len-2] == 'D' || novoProcesso[len-3] == '=')) // Real Time
	{
		//kill(pidEscalonador, SIGUSR2);
	}
	else // Round Robin
	{
		//kill(pidEscalonador, SIGALRM);
	}

	shmdt(novoProcesso);
}

pid_t addPrioridade(char* prog, int prioridade)
{
	char* vet[] = {prog, NULL};	
	pid_t pid;

	if ((pid = fork()) < 0) 
	{ 
		fprintf(stderr, "Erro ao criar filho\n"); 
		exit(-1); 
	} 
	if (pid == 0)
	{
		execv(prog, vet);
		exit(1);
	}
	kill(pid, SIGSTOP);

	return pid;
}

int main(int argc, char *argv[])
{
	printf("Escalonador iniciado\n");
	fflush(stdout);

	// REAL-TIME
	pid_t pidRealTime = -1;
	
	// PRIORIDADE
	Heap* heapPrioridade = heap_cria(MAX);
	pid_t pidPrioridade = -1;	
	
	// ROUND-ROBIN
	pid_t roundRobin[MAX]; 
	int numberRR = 0, currRR = 0;

	int i;

	for(i = 0; i<MAX; i++)
		roundRobin[i] = -1;	


	shmProcesso = atoi(argv[1]);
	signal(SIGUSR1, adicionarProcesso);

	while(1)
	{
		if(REAL_TIME)
		{

		}
		else if(PRIORIDADE)
		{
			heap_remove(heapPrioridade);
		}
		else if(ROUND_ROBIN)
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
