#ifndef HEAP_H
#define HEAP_H

#include <unistd.h>

typedef struct _heap Heap;

Heap* heap_cria(int max);
int  heap_insere(Heap* heap, float prioridade, pid_t processo);
pid_t heap_remove(Heap* heap);
void heap_show(Heap *heap, char* titulo);
Heap* heap_libera(Heap* heap);

#endif
