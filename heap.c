#include <stdlib.h>
#include <stdio.h>

#include "heap.h"

struct _heap {
	int max;           /* tamanho maximo do heap */
	int pos;           /* proxima posicao disponivel no vetor */
	float* prioridade; /* vetor das prioridades */
};


static void troca(int a, int b, float *v)
{
	float f = v[a];
	v[a] = v[b];
	v[b] = f;
}

static void corrige_acima(Heap* heap, int pos)
{
	while (pos > 0)
	{
		int pai = (pos - 1) / 2;
		if (heap->prioridade[pai] < heap->prioridade[pos])
			troca(pos, pai, heap->prioridade);
		else
			break;
		pos = pai;
	}
}

static void corrige_abaixo(Heap* heap)
{
	int pai = 0;
	while (2 * pai + 1 < heap->pos)
	{
		int filho_esq = 2 * pai + 1;
		int filho_dir = 2 * pai + 2;
		int filho;
		if (filho_dir >= heap->pos)
			filho_dir = filho_esq;
		if (heap->prioridade[filho_esq] > heap->prioridade[filho_dir])
			filho = filho_esq;
		else
			filho = filho_dir;
		if (heap->prioridade[pai] < heap->prioridade[filho])
			troca(pai, filho, heap->prioridade);
		else
			break;
		pai = filho;
	}
}

Heap* heap_cria(int max) 
{
	Heap* heap = (Heap*)malloc(sizeof(Heap));

	heap->max = max;
	heap->pos = 0;
	heap->prioridade = (float*)malloc(max*sizeof(float));

	return heap;
}



Heap* heap_libera(Heap* heap) 
{
	if (heap)
	{
		free(heap->prioridade);
		free(heap);
	}

	return heap;
}


int heap_insere(Heap* heap, float prioridade)
{
	if (heap->pos < heap->max)
	{
		heap->prioridade[heap->pos] = prioridade;
		corrige_acima(heap, heap->pos);
		heap->pos++;
		return 0;
	}
	else
		return 1;
}



float heap_remove(Heap* heap)
{
	if (heap->pos > 0)
	{
		float topo = heap->prioridade[0];
		heap->prioridade[0] = heap->prioridade[heap->pos - 1];
		heap->pos--;
		corrige_abaixo(heap);
		return topo;
	}
	else
		return -1;
}



void heap_show(Heap* heap, char* title) {
	int i;
	printf("%s={", title);
	for (i = 0; i<heap->pos; i++)
		printf("%g,", heap->prioridade[i]);
	printf("}\n");

}


Heap* heap_monta(int n, float* vector) {
	Heap* heap = heap_cria(n);
	
	int i;
	
	for (i = 0; i < n; i++)
		heap->prioridade[i] = vector[i];

	for (n; n > 0; n--) 
		corrige_acima(heap, n);
	
	heap->pos = heap->max;
	return heap;
}

void heap_sort(int n, float* vector) {
	int i = n / 2, pai, filho, t;

	while (1) {
		if (i > 0) {
			i--;
			t = vector[i];
		}
		else {
			n--;
			if (n == 0) return;
			t = vector[n];
			vector[n] = vector[0];
		}

		pai = i;
	
		filho = pai * 2 + 1;

		while (filho < n) {
			
			if ((filho + 1 < n) && (vector[filho + 1] > vector[filho]))
				filho++;
			if (vector[filho] > t) {
				vector[pai] = vector[filho];
				pai = filho;
				filho = pai * 2 + 1;
			}
			else {
				break;
			}
		}
		vector[pai] = t;
	}
}


