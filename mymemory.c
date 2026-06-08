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

// void mymemory free(mymemory t *memory, void *ptr): Libera a alocação apontada por ptr. 
// Se ptr nao for uma alocacão válida, a função não deve fazer nada.
void mymemory_free(mymemory_t *memory, void *ptr) {
    if (memory == NULL) {
        return;
    }
    
    // Se ptr nao for uma alocacão válida, a função não deve fazer nada.
    if (ptr == NULL) {
        return;
    }

    // cria um ponteiro current do tipo allocation_t e inicia ele apontando para o início da lista de alocações (head)
    allocation_t *current = memory->head;
    allocation_t *previous = NULL; // Ponteiro para rastrear o nó anterior, útil para remoção da lista

    // primeiro liberaa casa nó de alocação
    while (current != NULL) {
        if (current->start == ptr) {
            // Encontrou o bloco a ser liberado

            // se for o primeiro da lista, 
            if(previous == NULL) {
                // O bloco a ser liberado é o primeiro da lista, precisa atualizar o head para o próx nó na estrutura da memória
                memory->head = current->next; // Atualiza o head para o próximo nó
            } else {
                // O bloco a ser liberado está no meio ou no final da lista
                previous->next = current->next; // Pula o nó atual
            }
            
            // current->is_free = 1; // Marca o bloco como livre
            free(current); // Libera a estrutura de controle do bloco alocado;
            
            return;
        }
        // precisa atualizar pra nao ficar rpeso pra sempre no if
        previous = current;
        current = current->next;
    }
}