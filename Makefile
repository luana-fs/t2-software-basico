# Nome do compilador
CC = gcc

# Flags de compilação (avisos e debug para o Valgrind)
CFLAGS = -Wall -Wextra -g

# Nome do executável final
TARGET = my-lib

# Arquivos fonte
SRCS = mymemory.c main.c

# Regra padrão: executada ao digitar apenas "make"
all: $(TARGET)

# Regra para compilar e gerar o executável
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Regra para rodar o programa normalmente
run: $(TARGET)
	./$(TARGET)

# Regra para rodar com o Valgrind (verificação de memory leaks)
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

# Regra para limpar o executável gerado
clean:
	rm -f $(TARGET)