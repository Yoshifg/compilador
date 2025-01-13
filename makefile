CC = gcc
CFLAGS = -Wall -O2 -Iinclude

# Diretórios
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests/lexer_tests

# Arquivos fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

# Nome do alvo final
TARGET = main

# Arquivos de teste
TESTS = $(wildcard $(TEST_DIR)/*.txt)

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

# Regras para executar os testes do lexer, passando arquivos de teste como argumentos para o executável
$(TESTS): $(TARGET)
	./$(TARGET) $@

# Limpa arquivos objeto e o executável gerados, preparando o ambiente para uma nova compilação
clean:
	@rm -f $(OBJS) src/main.o $(TARGET)
