#ifndef MYMEMORY_H
#define MYMEMORY_H

#include <stddef.h>

// BLOCO
typedef struct allocation {
    void *start;
    size_t size; // tamanho em bytes do bloco alocado
    struct allocation *next; //por estarmos lidando com lista encadeada, guardamos o endereço do proximo nó no ponteiro next, se tentarmos percorrer somando 1 como em um array, isso gerará um erro de Segmentação, pois a proxima não vai estar necessariamente na posição seguinte a atual;
} allocation_t;

// MEMÓRIA
typedef struct {
    void *pool; // ponteiro para o bloco de memória real
    size_t total_size; // tamanho em bytes do bloco alocado
    allocation_t *head;
} mymemory_t;

mymemory_t* mymemory_init(size_t size);
void* mymemory_alloc(mymemory_t *memory, size_t size);
void mymemory_free(mymemory_t *memory, void *ptr);
void mymemory_display(mymemory_t *memory);
void mymemory_stats(mymemory_t *memory);
void mymemory_cleanup(mymemory_t *memory);

#endif /* MYMEMORY_H */