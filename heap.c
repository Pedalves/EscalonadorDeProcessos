#include <stdlib.h>
#include <stdio.h>

#include "heap.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _processo {
	float prioridade;
	char* processo;
}Processo;

struct _heap {
	int max;             /* tamanho maximo do heap */
	int pos;             /* proxima posicao disponivel no vetor */
	Processo** prioridade; /* vetor das prioridades */
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void troca(int a, int b, Processo **v)
{
	Processo* f = v[a];
	v[a] = v[b];
	v[b] = f;
}

static void corrige_acima(Heap* heap, int pos)
{
	while (pos > 0)
	{
		int pai = (pos - 1) / 2;
		if (heap->prioridade[pai]->prioridade > heap->prioridade[pos]->prioridade)
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
		if (heap->prioridade[filho_esq]->prioridade < heap->prioridade[filho_dir]->prioridade)
			filho = filho_esq;
		else
			filho = filho_dir;
		if (heap->prioridade[pai]->prioridade > heap->prioridade[filho]->prioridade)
			troca(pai, filho, heap->prioridade);
		else
			break;
		pai = filho;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Heap* heap_cria(int max) 
{
	Heap* heap = (Heap*)malloc(sizeof(Heap));

	heap->max = max;
	heap->pos = 0;
	heap->prioridade = (Processo**)malloc(max*sizeof(Processo*));

	return heap;
}



Heap* heap_libera(Heap* heap) 
{
	int i;

	if (heap)
	{
		for(i = 0; i<heap->max; i++)
			free(heap->prioridade[i]);

		free(heap->prioridade);
		free(heap);
	}

	return heap;
}


int heap_insere(Heap* heap, float prioridade, char* processo)
{
	if (heap->pos < heap->max)
	{
		Processo* novoProcesso = (Processo*)malloc(sizeof(Processo));
		novoProcesso->processo = processo;
		novoProcesso->prioridade = prioridade;

		heap->prioridade[heap->pos] = novoProcesso;
		corrige_acima(heap, heap->pos);
		heap->pos++;
		return 0;
	}
	else
		return 1;
}



char* heap_remove(Heap* heap)
{
	if (heap->pos > 0)
	{
		char* topo = heap->prioridade[0]->processo;
		heap->prioridade[0] = heap->prioridade[heap->pos - 1];
		heap->pos--;
		corrige_abaixo(heap);
		return topo;
	}
	else
		return NULL;
}



void heap_show(Heap* heap, char* title) {
	int i;
	printf("%s={", title);
	for (i = 0; i<heap->pos; i++)
		printf("%g %s,", heap->prioridade[i]->prioridade, heap->prioridade[i]->processo);
	printf("}\n");

}

