#include <stdio.h>
#include <stdlib.h>
#include "mymemory.h"

int main() {
    printf("==== TESTE GERENCIADOR DE MEMORIA ====\n");

    // 1. Inicializacao do pool
    mymemory_t *mem = mymemory_init(1024);
    if (!mem) {
        printf("Erro ao inicializar memoria\n");
        return 1;
    }

    // --------------------------------------------------
    // TESTE 1: ALOCACOES BASICAS
    // --------------------------------------------------
    printf("\n[TESTE 1] Alocacoes iniciais\n");

    void *a = mymemory_alloc(mem, 100);
    void *b = mymemory_alloc(mem, 200);
    void *c = mymemory_alloc(mem, 50);

    mymemory_display(mem);

    // --------------------------------------------------
    // TESTE 2: LIBERACAO E FRAGMENTACAO
    // --------------------------------------------------
    printf("\n[TESTE 2] Liberando bloco B (200 bytes)\n");
    mymemory_free(mem, b);

    mymemory_display(mem);

    // --------------------------------------------------
    // TESTE 3: REUTILIZACAO DE ESPACO (FIRST FIT)
    // --------------------------------------------------
    printf("\n[TESTE 3] Reutilizacao de espaco (First Fit)\n");

    void *d = mymemory_alloc(mem, 150); // deve entrar no buraco de B

    mymemory_display(mem);

    // --------------------------------------------------
    // TESTE 4: FRAGMENTACAO INTENCIONAL
    // --------------------------------------------------
    printf("\n[TESTE 4] Criando fragmentacao\n");

    mymemory_free(mem, a);
    mymemory_free(mem, c);

    mymemory_display(mem);

    // --------------------------------------------------
    // TESTE 5: ESTATISTICAS (EFICIENCIA)
    // --------------------------------------------------
    printf("\n[TESTE 5] Estatisticas de memoria\n");
    mymemory_stats(mem);

    // --------------------------------------------------
    // TESTE 6: VARIAS ALOCACOES/ LIBERACOES (STRESS)
    // --------------------------------------------------
    printf("\n[TESTE 6] Stress test\n");

    void *ptrs[10];

    for (int i = 0; i < 10; i++) {
        ptrs[i] = mymemory_alloc(mem, 30);
    }

    mymemory_display(mem);

    for (int i = 0; i < 10; i += 2) {
        mymemory_free(mem, ptrs[i]);
    }

    mymemory_display(mem);

    printf("\nEstatisticas apos stress:\n");
    mymemory_stats(mem);

    // --------------------------------------------------
    // LIMPEZA FINAL (IMPORTANTE PRA VALGRIND)
    // --------------------------------------------------
    printf("\n[FINAL] Limpando memoria\n");
    mymemory_cleanup(mem);

    printf("Finalizado.\n");

    return 0;
}
