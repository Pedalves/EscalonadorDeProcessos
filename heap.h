#ifndef HEAP_H
#define HEAP_H

typedef struct _heap Heap;

Heap* heap_cria(int max);
int  heap_insere(Heap* heap, float prioridade);
float heap_remove(Heap* heap);
void heap_show(Heap *heap, char* titulo);
Heap* heap_libera(Heap* heap);

/*
*  transforma um vertor qualquer num heap.
*  sua implementacao deve ser O(n).
*/
Heap* heap_monta(int n, float* vector);

/*
*  ordena um vetor em O(nlog(n)).
*/
void heap_sort(int n, float* vector);

#endif