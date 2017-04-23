#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int fileExec, fileIN, fileOUT; // arquivos de execução, entrada e saída
int pidEscalonador = -1;

int shmProcesso; // segmento para disparo de novos processos no escalonador

int openFile(char *name, int type) 
{
	int file;
	file = open(name, type, S_IRWXU|S_IRWXG|S_IRWXO);
	if(file == -1) {
		printf("[ERRO] Nao foi possivel abrir o arquivo %s", name);
		fflush(stdout);
		exit(-1);
	}
	return file;
}

void endHandler(int signal) 
{
	if(pidEscalonador != -1)
	{
		kill(SIGINT, pidEscalonador); // Encerra escalonador
	}

	close(fileExec);
	close(fileIN);
	close(fileOUT);

	shmctl(shmProcesso, IPC_RMID, 0); // Libera memória compartilhada

	exit(0);
}

void criaProcessoEscalonador(int fd[2]) {
	char argNomePrograma[11];

	shmProcesso = shmget(IPC_PRIVATE, sizeof(char) * 1024, IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR);

	pidEscalonador = fork(); // cria processo para o escalonador
	if(pidEscalonador == 0) // escalonador
	{
		close(fd[0]); // fecha pipe de leitura
		dup2(fd[1], 1); // redireciona stdout para pipe de escrita

		snprintf(argNomePrograma, 11, "%d", shmProcesso);
		char * execEscalonador[] = {"./escalonador", argNomePrograma, NULL};
		execve("./escalonador", execEscalonador, 0);
		exit(0);
	}
	else
	{
		dup2(fd[0], 0); // redireciona stdin para pipe de leitura
	}

	sleep(1);
}

void adicionaProcessos() {
	char buffer[2048];
	int cnt = 0, i, len;
	char *nomeProcesso;

	nomeProcesso = (char *) shmat(shmProcesso, 0, 0);

	while(read(fileExec,&buffer[cnt],1) != 0) {
		if(buffer[cnt] == '\n' && cnt > 1) {
			buffer[cnt] = '\0';
			strcpy(nomeProcesso, &buffer[5]);
			printf("-> Novo processo %s\n", nomeProcesso);
			fflush(stdout);

			len = strlen(nomeProcesso);

			kill(pidEscalonador, SIGUSR1);

			sleep(1);
			buffer[0] = '\0';
			cnt = 0;
		}
	cnt++;
	}

	shmdt(nomeProcesso);
}

int main(void) {
	int fd[2]; // pipe

	fileExec = openFile("exec.txt", O_RDONLY); // Abre arquivo para leitura
	fileIN = openFile("entrada.txt", O_RDONLY); // Abre arquivo para leitura
	fileOUT = openFile("saida.txt", O_CREAT|O_RDWR|O_TRUNC); // Abre arquivo para escrita

	signal(SIGINT, endHandler);

	if(pipe(fd) < 0) { // abre pipe
		printf("Nao foi possivel abrir o pipe\n");
		fflush(stdout);
		exit(-1);
	}

	if(dup2(fileIN, 0) == -1) { // redireciona stdin para arquivo de entrada
		printf("Nao foi possivel redirecionar o stdin para arquivo de entrada\n");
		fflush(stdout);
		exit(-1);
	}

	if(dup2(fileOUT, 1) == -1) { // redireciona stdout para arquivo de saída
		printf("Nao foi possivel redirecionar o stdout para arquivo de saida\n");
		fflush(stdout);
		exit(-1);
	}
	if(dup2(fileOUT, 2) == -1) { // redireciona stderr para arquivo de saída
		printf("Nao foi possivel redirecionar o stderr para arquivo de saida\n");
		fflush(stdout);
		exit(-1);
	}

	close(fd[1]); // fecha pipe de escrita

	criaProcessoEscalonador(fd);
	adicionaProcessos();
	while(1) sleep(1);
	waitpid(pidEscalonador, NULL, 0);

	close(fileExec);
	close(fileIN);
	close(fileOUT);

	shmctl(shmProcesso, IPC_RMID, 0);

	return 0;
}
