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
typedef struct _realTime{
	int inicio;
	int duracao;

	pid_t pid;
}RealTime;

RealTime* realTimeArray[MAX];
pid_t pidRealTime = -1;
int numberRT = 0, currRT = 0, minutoPercorrido = 0;

// PRIORIDADE
Heap* heapPrioridade;
Processo* currPrioridade; // Struct processo da heap
pid_t pidPrioridade = -1;	

// ROUND-ROBIN
pid_t roundRobin[MAX]; 
int numberRR = 0, currRR = 0;

int shmProcesso; // segmento para disparo de novos processos no escalonador

void addRealTime(char* prog, int duracao, int inicio)
{
	char* vet[] = {prog, NULL};	
	pid_t pid;
	int i, j;
	RealTime* temp;

	if(duracao + inicio > 60)
	{
		printf("-> Duracao + inicio > 60\n");
		fflush(stdout);
		return;
	}

	for(i = 0; i<MAX; i++)
	{
		if(realTimeArray[i] != NULL)
		{
			if(realTimeArray[i]->inicio <= inicio && inicio < realTimeArray[i]-> inicio + realTimeArray[i]->duracao)
			{
				printf("-> Conflito de tempo\n");
				fflush(stdout);
				return;
			}
			if(inicio <= realTimeArray[i]->inicio && realTimeArray[i]->inicio < inicio + duracao)
			{
				printf("-> Conflito de tempo\n");
				fflush(stdout);
				return;
			}
		}
	}

	if ((pid = fork()) < 0) 
	{ 
		fprintf(stderr, "Erro ao criar filho\n"); 
		exit(-1); 
	} 
	if (pid == 0)
	{
		execv(prog, vet);
		return;
	}

	printf("-> Real-Time inserido com PID %d\n", pid);
	fflush(stdout);

	realTimeArray[numberRT] = (RealTime*) malloc(sizeof(RealTime));
	realTimeArray[numberRT]->inicio = inicio;
	realTimeArray[numberRT]->duracao = duracao;
	realTimeArray[numberRT]->pid = pid;

	numberRT++;

	j=0;
	for(i = numberRT; i<j; i--)
	{
		if(realTimeArray[i-1] == NULL)
		{
			realTimeArray[i-1] = realTimeArray[i];
			realTimeArray[i] = NULL;				
		}
		else if(realTimeArray[i-1]->inicio < realTimeArray[i]->inicio)
		{
			temp = realTimeArray[i-1];
			realTimeArray[i-1] = realTimeArray[i];
			realTimeArray[i] = temp;
		}

		j++;
	}
	fflush(stdout);
	kill(pid, SIGSTOP);
	REAL_TIME++;
}

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

	printf("-> Round-Robin inserido com PID %d\n", pid);
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
		execv(prog, vet);
		exit(0);
	}
	kill(pid, SIGSTOP);
	printf("-> Prioridade inserida com PID %d\n", pid);
	fflush(stdout);
	heap_insere(heapPrioridade, prioridade, pid);

	PRIORIDADE++;
}

void addHandler(int signal) 
{
	char *novoProcesso; // Nome do novo processo
	char nome[20];

	int i, len, prioridade;
	char msgCriacao[200];

	novoProcesso = (char *) shmat(shmProcesso, 0, 0);

	sscanf(novoProcesso, "%s", nome);

	printf("-> %s recebido no escalonador\n", nome);
	fflush(stdout);

	len = strlen(novoProcesso);

	if(len > 3 && novoProcesso[len-3] == 'P') // Prioridade
	{
		addPrioridade(nome, (int)novoProcesso[len-1] - 48);
	}
	else if(len > 3 && novoProcesso[len-3] == '=') // Real Time
	{
		addRealTime(nome, ((int)novoProcesso[len-1]-48) + ((int)novoProcesso[len-2]-48)*10, ((int)novoProcesso[len-6]-48) + ((int)novoProcesso[len-7] - 48)*10);
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
	{
		roundRobin[i] = -1;	
		realTimeArray[i] = NULL;
	}

	shmProcesso = atoi(argv[1]);
	signal(SIGUSR1, addHandler);	

	while(1)
	{
		if(REAL_TIME)
		{
			if(PRIORIDADE)
			{
				if(currPrioridade != NULL) // Para prioridade caso entre Real Time
				{
					kill(currPrioridade->processo, SIGSTOP);
					heap_insere(heapPrioridade, currPrioridade->prioridade, currPrioridade->processo);
					pidPrioridade = -1;

					currPrioridade = NULL;
				}
			}
			if(realTimeArray[currRT] != NULL)
			{	
				if((realTimeArray[currRT]->inicio - minutoPercorrido) > 60)
				{
					minutoPercorrido = 0;
				}
				minutoPercorrido = realTimeArray[currRT]->inicio - minutoPercorrido;
				
				printf("-> Faltam %ds para proxima execucao \n", minutoPercorrido);
				fflush(stdout);

				sleep(minutoPercorrido);

				kill(realTimeArray[currRT]->pid, SIGCONT);
				sleep(realTimeArray[currRT]->duracao);
				
				minutoPercorrido = realTimeArray[currRT]->inicio + realTimeArray[currRT]->duracao;
				kill(realTimeArray[currRT]->pid, SIGSTOP);

				if(waitpid(realTimeArray[currRT]->pid, NULL, WNOHANG) != 0)
				{
					printf("Processo %d finalizado\n", realTimeArray[currRT]->pid);
					fflush(stdout);

					free(realTimeArray[currRT]);
					realTimeArray[currRT] = NULL;

					REAL_TIME--;
				}
			}

			if(currRT < numberRT)
				currRT++;
			else
			{
				minutoPercorrido = 60 - minutoPercorrido;
				printf("-> Faltam %ds para o fim do minuto\n", minutoPercorrido);
				fflush(stdout);

				sleep(minutoPercorrido);

				minutoPercorrido = 0;
				currRT = 0;
			}

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
				printf("Processo %d finalizado\n", pidPrioridade);
				fflush(stdout);

				pidPrioridade = -1;
				PRIORIDADE--;
			}
			else if(currPrioridade != NULL)
			{
				if(currPrioridade->prioridade > heap_prioridade(heapPrioridade) && heap_prioridade(heapPrioridade) != -1)
				{
					printf("-> Prioridade maior foi inserida\n");
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
					printf("Processo %d finalizado\n", roundRobin[currRR]);
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
