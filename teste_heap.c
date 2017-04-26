#include <stdio.h>
#include "heap.h"


int main(void) {
	int i, n = 8;
	float v[] = { 25, 48, 37, 12, 57, 86, 33, 92 };
	char* c[] = { "A", "B", "C", "D", "E", "F", "G", "H" };
	char* info;	

	Heap* heap = heap_cria(30);
	for (i = 0; i<n; i++) {
		heap_insere(heap, v[i], c[i]);
		printf("insere %g, ", v[i]);
		heap_show(heap, "heap=");
	}
	info = heap_remove(heap);
	for(i = 0; i<7; i++)
	{
		printf("remove %s ", info);
		heap_show(heap, "heap=");
		info = heap_remove(heap);
	}

	return 0;
}
