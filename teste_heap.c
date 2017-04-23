#include <stdio.h>
#include "heap.h"


int main(void) {
	int i, n = 8;
	float info, v[] = { 25, 48, 37, 12, 57, 86, 33, 92 };
	heap_sort(n, v);
	for (i = 0; i < n; i++)
		printf("%.1f ", v[i]);
	/*Heap* heap = heap_cria(30);
	for (i = 0; i<n; i++) {
		heap_insere(heap, v[i]);
		printf("insere %g, ", v[i]);
		heap_show(heap, "heap=");
	}
	info = heap_remove(heap);
	while (info>0)
	{
		printf("remove %g ", info);
		heap_show(heap, "heap=");
		info = heap_remove(heap);
	}*/

	/*Heap* heap = heap_monta(n ,v);
	
	heap_show(heap, "heap=");

	heap = heap_libera(heap);*/
	return 0;
}