# MyMemory - Simulador de Gerenciamento de Memória em C

## Descrição

O projeto MyMemory implementa um gerenciador simplificado de memória em linguagem C, simulando parte do comportamento das funções malloc() e free() da biblioteca padrão.

A proposta consiste em reservar um grande bloco contínuo de memória (pool) e administrar manualmente as alocações realizadas dentro desse espaço. Para controlar os blocos ocupados, é utilizada uma lista encadeada contendo informações sobre cada alocação.

O projeto foi desenvolvido como exercício de fixação dos conceitos de:

- Ponteiros
- Alocação dinâmica de memória
- Estruturas (struct)
- Listas encadeadas
- Modularização em C
- Gerenciamento manual de memória
- Estratégia de alocação First Fit

---

## Estrutura do Projeto

text . ├── main.c ├── mymemory.c ├── mymemory.h ├── Makefile └── README.md 

### Arquivos

#### mymemory.h

Contém:

- definição das estruturas;
- protótipos das funções públicas da biblioteca.

#### mymemory.c

Implementa toda a lógica do gerenciador:

- inicialização;
- alocação;
- liberação;
- estatísticas;
- exibição;
- limpeza.

#### main.c

Programa de testes.

#### Makefile

Automatiza compilação, execução e análise com Valgrind.

---

## Estruturas Utilizadas

### allocation_t

Representa uma alocação individual dentro do pool.

c typedef struct allocation {     void *start;     size_t size;     struct allocation *next; } allocation_t; 

Campos:

| Campo | Descrição |
|---------|---------|
| start | endereço inicial do bloco |
| size | tamanho do bloco em bytes |
| next | próximo nó da lista |

---

### mymemory_t

Representa o gerenciador de memória.

c typedef struct {     void *pool;     size_t total_size;     allocation_t *head; } mymemory_t; 

Campos:

| Campo | Descrição |
|---------|---------|
| pool | início do bloco total de memória |
| total_size | tamanho do pool |
| head | início da lista de alocações |

---

## Funcionamento

### Inicialização

c mymemory_t *memory = mymemory_init(1024); 

Cria:

1. uma estrutura de controle (mymemory_t);
2. um pool de memória de 1024 bytes.

---

### Alocação

c void *ptr = mymemory_alloc(memory, 100); 

A função procura o primeiro espaço livre capaz de armazenar os 100 bytes solicitados.

A estratégia utilizada é:

### First Fit

O algoritmo percorre os blocos ocupados e utiliza o primeiro espaço livre suficientemente grande.

---

### Liberação

c mymemory_free(memory, ptr); 

Remove o bloco da lista de alocações.

O espaço volta a ficar disponível para futuras alocações.

---

### Estatísticas

c mymemory_stats(memory); 

Exibe:

- quantidade de alocações;
- memória ocupada;
- memória livre;
- maior bloco livre;
- quantidade de fragmentos livres.

---

### Mapa de Alocações

c mymemory_display(memory); 

Exibe a posição e o tamanho de cada bloco ocupado.

Exemplo:

text --- Mapa de Alocacoes --- Bloco 1: Inicio no byte 0 | Tamanho: 100 bytes Bloco 2: Inicio no byte 200 | Tamanho: 50 bytes ------------------------- 

---

### Limpeza

c mymemory_cleanup(memory); 

Libera:

- todos os nós da lista;
- o pool de memória;
- a estrutura de controle.

Evita vazamentos de memória (memory leaks).

---

## Compilação

Compilar o projeto:

bash make 

---

## Execução

Executar o programa:

bash make run 

---

## Verificação com Valgrind

Executar análise de memória:

bash make valgrind 

O Valgrind verifica:

- memory leaks;
- acessos inválidos;
- erros relacionados a memória dinâmica.

---

## Limpeza do Projeto

Remover o executável gerado:

bash make clean 

---

## Conceitos da Disciplina Aplicados

Este projeto utiliza diversos conceitos estudados em Programação de Software Básico:

- Ponteiros e endereços de memória
- Operadores * e &
- Estruturas (struct)
- Listas encadeadas
- Heap e Stack
- Alocação dinâmica com malloc()
- Liberação de memória com free()
- Modularização (.h e .c)
- Compilação com GCC
- Automação com Makefile
- Depuração com Valgrind

---

## Autores

Luana, Natália e Raffaella.
Projeto desenvolvido para a disciplina de Programação de Software Básico (2026/1)
PUCRS - Engenharia de Software