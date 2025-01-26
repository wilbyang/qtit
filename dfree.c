#include <stdio.h>
#include <stdlib.h>

int main() {
    int* p = (int*)malloc(sizeof(int));
    char* q = (char*)p;

    free(p);
    free(q);
}
//gcc dfree.c -fsanitize=address -o dfree