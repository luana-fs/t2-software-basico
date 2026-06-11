#include <stdio.h>
#include <stdlib.h>
#include "mymemory.h"

int main() {
    size_t pool_size;

    printf("Digite o tamanho do pool em bytes: ");
    scanf("%zu", &pool_size);

    mymemory_t *gerenciador = mymemory_init(pool_size);

    if (gerenciador == NULL) {
        printf("Erro ao criar gerenciador.\n");
        return 1;
    }

    printf("Pool de %zu bytes criado com sucesso.\n", pool_size);

    int opcao = -1;

    while (opcao != 0) {
        printf("\n--- Menu ---\n");
        printf("1. Alocar bloco\n");
        printf("2. Liberar bloco\n");
        printf("3. Exibir alocacoes\n");
        printf("4. Exibir estatisticas\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            size_t size;

            printf("Digite o tamanho do bloco: ");
            scanf("%zu", &size);

            void *ptr = mymemory_alloc(gerenciador, size);

            if (ptr == NULL) {
                printf("Falha ao alocar %zu bytes.\n", size);
            } else {
                size_t offset = (size_t)ptr - (size_t)gerenciador->pool;
                printf("Bloco alocado com sucesso no byte %zu.\n", offset);
            }

        } else if (opcao == 2) {
            size_t offset;

            printf("Digite o byte inicial do bloco a liberar: ");
            scanf("%zu", &offset);

            void *ptr = (char *)gerenciador->pool + offset;

            mymemory_free(gerenciador, ptr);

            printf("Tentativa de liberacao realizada.\n");

        } else if (opcao == 3) {
            mymemory_display(gerenciador);

        } else if (opcao == 4) {
            mymemory_stats(gerenciador);

        } else if (opcao == 0) {
            printf("Encerrando...\n");

        } else {
            printf("Opcao invalida.\n");
        }
    }

    mymemory_cleanup(gerenciador);

    printf("Recursos liberados com sucesso.\n");

    return 0;
}