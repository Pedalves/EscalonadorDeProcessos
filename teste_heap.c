#include <stdio.h>
#include "heap.h"


int main(void) {
	int i, n = 8;
	float v[] = { 25, 48, 37, 12, 57, 86, 33, 92 };
	int info;	

	Heap* heap = heap_cria(30);
	for (i = 0; i<n; i++) {
		heap_insere(heap, v[i], i);
		printf("insere %g, ", v[i]);
		heap_show(heap, "heap=");
	}
	info = heap_remove(heap);
	for(i = 0; i<7; i++)
	{
		printf("remove %d ", info);
		heap_show(heap, "heap=");
		info = heap_remove(heap);
	}

	return 0;
}
