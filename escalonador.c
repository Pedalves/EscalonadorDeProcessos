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

// REAL-TIME
pid_t pidRealTime = -1;

// PRIORIDADE
Heap* heapPrioridade;
Processo* currPrioridade; // Struct processo da heap
pid_t pidPrioridade = -1;	

// ROUND-ROBIN
pid_t roundRobin[MAX]; 
int numberRR = 0, currRR = 0;

int shmProcesso; // segmento (shm) para disparo de novos processos no escalonador

void addRoundRobin(char* prog)
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
		raise(SIGSTOP);
		execv(prog, vet);
		return;
	}

	printf("-> Round-Robin inserido\n");
	fflush(stdout);

	roundRobin[numberRR] = pid;
	numberRR++;

	ROUND_ROBIN++;
}

void addPrioridade(char* prog, int prioridade)
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
		//raise(SIGSTOP);
		execv(prog, vet);
		exit(0);
	}
	kill(pid, SIGSTOP);
	printf("-> Prioridade inserida\n");
	fflush(stdout);
	heap_insere(heapPrioridade, prioridade, pid);

	PRIORIDADE++;
}

void addHandler(int signal) 
{
	// if(currPrioridade != NULL)
	// {	

	// 	printf("-> Para a prioridade\n");
	// 	fflush(stdout);
	// 	//kill(currPrioridade->processo, SIGSTOP);
	// }

	char *novoProcesso; // Nome do novo processo (shm)
	char nome[20];

	int i, len, prioridade;
	char msgCriacao[200];

	novoProcesso = (char *) shmat(shmProcesso, 0, 0);

	sscanf(novoProcesso, "%s", nome);

	printf("-> %s recebido no escalonador\n", nome);
	fflush(stdout);

	len = strlen(novoProcesso);

	if(len > 3 && novoProcesso[len-2] == '=' && novoProcesso[len-3] == 'R') // Prioridade
	{
		addPrioridade(nome, (int)novoProcesso[len-1] - 48);
	}
	else if(len > 3 && (novoProcesso[len-2] == 'D' || novoProcesso[len-3] == '=')) // Real Time
	{
		//kill(pidEscalonador, SIGUSR2);
	}
	else // Round Robin
	{
		addRoundRobin(nome);
	}

	shmdt(novoProcesso);
}

int main(int argc, char *argv[])
{
	printf("-> Escalonador iniciado\n");
	fflush(stdout);

	int i;

	heapPrioridade = heap_cria(MAX);
	currPrioridade = NULL;

	for(i = 0; i<MAX; i++)
		roundRobin[i] = -1;	


	shmProcesso = atoi(argv[1]);
	signal(SIGUSR1, addHandler);	

	while(1)
	{

			if(REAL_TIME)
		{

		}
		else if(PRIORIDADE)
		{
			
			if(pidPrioridade == -1){
			currPrioridade = heap_remove(heapPrioridade);
			
			pidPrioridade = currPrioridade->processo;

			kill(currPrioridade->processo, SIGCONT);
			}
			else if(waitpid(pidPrioridade, NULL, WNOHANG) != 0)
			{
				printf("Processo finalizado\n");
				fflush(stdout);

				pidPrioridade = -1;
				PRIORIDADE--;
			}
			else if(currPrioridade != NULL)
			{
				if(currPrioridade->prioridade > heap_prioridade(heapPrioridade) && heap_prioridade(heapPrioridade) != -1)
				{
					printf("->Prioridade maior foi inserida\n");
					fflush(stdout);
					kill(currPrioridade->processo, SIGSTOP);
					heap_insere(heapPrioridade, currPrioridade->prioridade, currPrioridade->processo);
					pidPrioridade = -1;
				}
			}

		}
		else if(ROUND_ROBIN)
		{
			if(roundRobin[currRR] != -1)
			{
				kill(roundRobin[currRR], SIGCONT);
				sleep(0.5);
				kill(roundRobin[currRR], SIGSTOP);

				if(waitpid(roundRobin[currRR], NULL, WNOHANG) != 0)
				{
					printf("Processo finalizado\n");
					fflush(stdout);

					roundRobin[currRR] = -1;

					ROUND_ROBIN--;
				}
			}

			if(currRR < numberRR)
				currRR++;
			else
				currRR = 0;
		}
	}

	return 0;
}
