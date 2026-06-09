#include <stdio.h>
#include <stdlib.h>
#include "mymemory.h"

// aluga um blocao de memória do pool para o usuário, se houver espaço suficiente, 
// pra então precisa procurar um espaço livre no pool e alocar um bloco menor allocation_t de memória para o usuário
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
// recebe o pool de memória e o ponteiro para o bloco a ser liberado, e então precisa procurar esse bloco na lista de alocações para remover
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
        // precisa atualizar pra prosseguir com a iteração
        previous = current;
        current = current->next;
    }
}

// void mymemory cleanup(mymemory t *memory): Libera todos os recursos (incluindo todas as aloca¸c˜oes e o bloco de mem´oria total).
void mymemory_cleanup(mymemory_t *memory) {
    if (memory == NULL) {
        return;
    }

    // limpa lista encadeada
    allocation_t *current = memory->head;

    while (current != NULL) {
        // salvar o endereço de next antes de dar free e perder a referencia
        allocation_t *next = current->next;

        // libera o bloco de alocação atual
        free(current);

        // atualiza o current para o next que foi salvo antetiormente
        current = next;
    }

    // Libera o pool de memória
    if (memory->pool != NULL) {
        free(memory->pool);
    }

    // Libera a estrutura de controle
    free(memory);
}


void mymemory_stats(mymemory_t *memory) {
    if (memory == NULL) {
        return;
    }

    int total_allocations = 0;
    size_t total_allocated_memory = 0; // Inicialmente, toda a memória está livre
    size_t max_free_blocks = 0;
    int free_fragments = 0;

    char *current_pool_addr = (char *)memory->pool;
    char *pool_end = current_pool_addr + memory->total_size;
    
    // bloco inicial
    allocation_t *current = memory->head;

    while (current != NULL) {
        // soma um bloco
       total_allocations++;
       // soma o size em bytes de cada bloco
       total_allocated_memory += current->size; // Subtrai o tamanho do bloco al
    
        // verifica se há gap dem branco do inicio do bloco head até o inicio da memoria
        char *start_addr = (char *)current->start;
        size_t gap = start_addr - current_pool_addr;

        if (gap > 0) {
            free_fragments++; // Achou um fragmento de memória livre [cite: 44]
            if (gap > max_free_blocks) {
                max_free_blocks = gap; // Atualiza o maior bloco [cite: 43]
            }
        }

        // pula o ponteiro para o fim do bloco atual
        current_pool_addr = start_addr + current->size;

        current = current->next;
    }

    printf("Total Memory: %zu bytes\n", memory->total_size);
    printf("Allocated Memory: %zu bytes\n", total_allocations);
    printf("Free Memory: %zu bytes\n",  total_allocated_memory);
}