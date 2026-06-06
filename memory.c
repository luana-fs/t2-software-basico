#include <stdio.h>
#include <stdlib.h>
#include "mymemory.h"

mymemory_t* mymemory_init(size_t size) {
    // 1. Aloca espaço para apenas para a estrutura de controle mymemory_t, tamanho muito pequeno
    // além de alocar faz o parse para memory_t
    // Criar o local onde você vai "anotar" os dados do seu gerenciador. 
    // Sem esse passo, você não teria onde guardar as informações sobre a sua memória
    mymemory_t *manager = (mymemory_t *)malloc(sizeof(mymemory_t));

    // se o SO negar memória, retornamos NULL para indicar falha na inicialização
    if (manager == NULL) {
        return NULL; // Proteção caso o SO negue a memória
    }

    // 2. Aloca o grande bloco contíguo de memória (o pool) que é entregue ao usuário e guarda dentro da estrutura de controle criada acima
    manager->pool = malloc(size);
    if (manager->pool == NULL) {
        free(manager); // Evita memory leak da estrutura de controle se o pool falhar
        return NULL;
    }

    // 3. Configura os valores iniciais
    manager->total_size = size;
    manager->head = NULL; // A lista começa vazia, pois ainda não há blocos ocupados

    return manager;
}
