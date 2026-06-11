# Nome do compilador
CC = gcc

# Flags de compilação
# -Wall   : avisos importantes
# -Wextra : avisos adicionais
# -g      : informações de debug
CFLAGS = -Wall -Wextra -g

# Nome do executável final
TARGET = my-lib

# Arquivos fonte
SRCS = mymemory.c main.c

# Regra padrão
all: $(TARGET)

# Compilação
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Execução
run: $(TARGET)
	./$(TARGET)

# Verificação com Valgrind (Linux)
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

# Limpeza
clean:
	rm -f $(TARGET)