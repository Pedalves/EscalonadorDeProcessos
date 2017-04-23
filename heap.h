#ifndef HEAP_H
#define HEAP_H

#include <unistd.h>

typedef struct _processo {
	int prioridade;
	pid_t processo;
}Processo;

typedef struct _heap Heap;

Heap* heap_cria(int max);
int  heap_insere(Heap* heap, int prioridade, pid_t processo);
Processo* heap_remove(Heap* heap);
void heap_show(Heap *heap, char* titulo);
Heap* heap_libera(Heap* heap);

int heap_prioridade(Heap* heap);

#endif
