#ifndef HEAP_H
#define HEAP_H

typedef struct _heap Heap;

Heap* heap_cria(int max);
int  heap_insere(Heap* heap, float prioridade, char* processo);
char* heap_remove(Heap* heap);
void heap_show(Heap *heap, char* titulo);
Heap* heap_libera(Heap* heap);

#endif
