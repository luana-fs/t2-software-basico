#include <stdio.h>
#include <stdlib.h>
#include "mymemory.h"

int main() {
    printf("--- Iniciando teste do mymemory_init ---\n");

    // Tenta alocar um pool de 1024 bytes (1 KB)
    size_t tamanho_desejado = 1024;
    mymemory_t *gerenciador = mymemory_init(tamanho_desejado);

    // Verifica se a alocação falhou (o SO negou a memória)
    if (gerenciador == NULL) {
        printf("ERRO: Falha ao inicializar o gerenciador de memória.\n");
        return 1;
    }

    // Imprime os resultados para confirmar que tudo está no lugar
    printf("- Endereço da prancheta (gerenciador): %p\n", (void*)gerenciador);
    printf("- Endereço do pool gigante (pool):     %p\n", gerenciador->pool);
    printf("- Tamanho total registrado:            %zu bytes\n", gerenciador->total_size);
    printf("- Início da lista (head):              %p\n", (void*)gerenciador->head);

    // IMPORTANTE: Como ainda não implementamos a função mymemory_cleanup(),
    // vamos dar o free() manualmente aqui no main apenas para o teste passar no Valgrind.
    free(gerenciador->pool);
    free(gerenciador);
    
    return 0;
}