#include <stdio.h>
#include <stdlib.h>
#include "mymemory.h"

int main() {

    mymemory_t *gerenciador = mymemory_init(1024);

    if (gerenciador == NULL) {
        printf("Erro ao criar gerenciador.\n");
        return 1;
    }

    printf("Pool criado com sucesso.\n");

    void *a = mymemory_alloc(gerenciador, 100);
    void *b = mymemory_alloc(gerenciador, 200);
    void *c = mymemory_alloc(gerenciador, 50);

    mymemory_display(gerenciador);

    printf("\nLiberando bloco B...\n");

    mymemory_free(gerenciador, b);

    mymemory_display(gerenciador);

    printf("\nEstatisticas:\n");

    mymemory_stats(gerenciador);

    mymemory_cleanup(gerenciador);

    return 0;
}