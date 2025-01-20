CC = gcc
CFLAGS = -Wall -O2 -Iinclude

# Diretórios
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIRS = tests/lexer_tests tests/parser_tests tests/semantic_analysis

# Arquivos fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

# Nome do alvo final
TARGET = main

# Arquivos de teste
TESTS = $(wildcard $(foreach dir, $(TEST_DIRS), $(dir)/*.txt))

# Alvo principal
all: $(TARGET)

# Regra para gerar o executável 'main' a partir dos arquivos objeto 'main.o' e outros objetos definidos em '$(OBJS)'
$(TARGET): src/main.o $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

# Regra para compilar 'main.c' em 'main.o', gerando o arquivo objeto necessário para o executável
src/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

# Regra para compilar arquivos '.c' em arquivos '.o', considerando as dependências dos cabeçalhos '.h'
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para executar um teste específico
run-test: $(TARGET)
	@if [ -z "$(TEST)" ]; then \
		echo "Usage: make run-test TEST=<path/to/test_file.txt>"; \
	else \
		echo "Running test: $(TEST)"; \
		./$(TARGET) $(TEST); \
	fi

# Limpa arquivos objeto e o executável gerados, preparando o ambiente para uma nova compilação
clean:
	@rm -f $(OBJS) src/main.o $(TARGET)
