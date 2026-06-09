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
    if (!manager) { //troquei a logica do if só por conveniencia - raffa
        return NULL; // Proteção caso o SO negue a memória
    }

    // 2. Aloca o grande bloco contíguo de memória (o pool) que é entregue ao usuário e guarda dentro da estrutura de controle criada acima
    manager->pool = malloc(size);

    //Se falhar ao alocar o blocão, precisamos liberar a estrutura gerencial para evitar memory leak
    if (!manager->pool) {//tambem troquei a logica para o uso de not por conveniencia - raffa
        free(manager); // Evita memory leak da estrutura de controle se o pool falhar
        return NULL;
    }

    // 3. Configura os valores iniciais
    manager->total_size = size;
    manager->head = NULL; // A lista começa vazia, pois ainda não há blocos ocupados

    return manager;
}

// Função que simula o malloc do C usando a estratégia Primeiro Ajuste (First Fit)
void* mymemory_alloc(mymemory_t *memory, size_t size) {
    // 1. Validações iniciais: o pool existe? o tamanho pedido é 0? é maior que o pool inteiro?
    if (!memory || size == 0 || size > memory->total_size) return NULL;

    void *alloc_start = NULL;        // Guarda o endereço onde o novo bloco vai começar
    allocation_t *curr = memory->head; // Ponteiro auxiliar para percorrer a lista encadeada

    // CASO 1: O pool está completamente vazio.
    if (curr == NULL) {
        // O bloco pode começar exatamente no início do pool
        alloc_start = memory->pool;
    }else{
        // CASO 2: Verificar se há um "buraco" LOGO NO INÍCIO, antes da primeira alocação
        // Convertendo ponteiros para size_t (números inteiros) para podermos fazer contas matemáticas
        size_t gap_start = (size_t)memory->pool; 
        size_t first_alloc_start = (size_t)curr->start;
        
        // Se a diferença de bytes entre o início do pool e o início do primeiro bloco ocupado couber o 'size'
        if (first_alloc_start - gap_start >= size) {
            alloc_start = memory->pool; // Alocamos logo no começo
        }else {
            // CASO 3: O início está ocupado. Vamos procurar buracos ENTRE os blocos.
            while (curr != NULL) {
                // current_end é o byte exato onde o bloco atual termina
                size_t current_end = (size_t)curr->start + curr->size; //lembrando que não precisamos fazer o type casting no curr->size pois size já é declarado na struct (mymemory.h) como size_t, não como ponteiro.
                
              
                // Variável para guardar onde o próximo bloco começa
                size_t next_start;

                // Verifica se existe um próximo bloco na lista
                if (curr->next != NULL) {
                    // Se existe, o limite é o início desse próximo bloco
                    next_start = (size_t)curr->next->start;
                } else {
                    // Se NÃO existe (ou seja, é o último bloco), o limite é o final do pool inteiro
                    next_start = (size_t)memory->pool + memory->total_size;
                }


                // Se o espaço vazio entre o fim do bloco atual e o início do próximo for suficiente...
                if (next_start - current_end >= size) {
                    // Achamos um espaço! O endereço de início será exatamente onde o bloco atual termina.
                    alloc_start = (void*)current_end;
                    break; // Este é o FIRST FIT: paramos de procurar assim que achamos o primeiro buraco válido.
                }
                
                // Pula para o próximo nó da lista para continuar procurando
                curr = curr->next;
            }
        }
    }

    // Se varremos tudo e alloc_start continua NULL, significa que não há espaço contíguo suficiente.
    if (!alloc_start) return NULL; 

    // --- FASE DE CRIAÇÃO DO NÓ DE CONTROLE ---
    
    // Aloca um novo nó da lista encadeada para representar essa nova região de memória ocupada
    allocation_t *new_alloc = (allocation_t*) malloc(sizeof(allocation_t)); //permitido usar malloc para controle de nós
    new_alloc->start = alloc_start; // Guarda o endereço de memória fatiado
    new_alloc->size = size;         // Guarda o tamanho solicitado
    new_alloc->next = NULL;         // Inicialmente não aponta para ninguém

    // --- FASE DE INSERÇÃO ORDENADA NA LISTA ---
    // Precisamos manter a lista ordenada pelo endereço de memória para que a busca de buracos funcione.
    
    // Se a lista estiver vazia ou o novo bloco for ficar fisicamente ANTES do primeiro bloco atual
    if (memory->head == NULL || (size_t)alloc_start < (size_t)memory->head->start) {
        new_alloc->next = memory->head; // O novo nó aponta para o antigo cabeça
        memory->head = new_alloc;       // O novo nó vira o cabeça da lista
    } else {
        // Se for no meio ou no fim, procuramos a posição correta
        curr = memory->head;
        // Avançamos enquanto o próximo nó existir E o endereço dele for menor que o endereço do nosso novo bloco
        while (curr->next != NULL && (size_t)curr->next->start < (size_t)alloc_start) {
            curr = curr->next;
        }
        // Ao sair do while, achamos a posição. Inserimos o 'new_alloc' entre 'curr' e 'curr->next'
        new_alloc->next = curr->next;
        curr->next = new_alloc;
    }

    // Retorna o ponteiro simulando o malloc. O usuário poderá gravar dados a partir deste endereço.
    return alloc_start;
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