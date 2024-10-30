# Compilador e flags
CC = gcc
CFLAGS = -O2 -march=native -Iinclude

# Nome do alvo final
TARGET = main

# Arquivos de comparação
OUTPUT_FILE = saida.dat
GOLDEN_FILE = gabarito.dat

# Alvo principal
all: $(TARGET)

# Regra para gerar o executável
$(TARGET): src/main.o src/read.o src/token.o
	@$(CC) $(CFLAGS) -o $(TARGET) src/main.o src/read.o src/token.o
	@./$(TARGET)

# Regra para gerar o arquivo de objeto main.o
src/main.o: src/main.c include/read.h include/token.h
	@$(CC) $(CFLAGS) -c src/main.c -o src/main.o

# Regra para gerar o arquivo de objeto read.o
src/read.o: src/read.c include/read.h
	@$(CC) $(CFLAGS) -c src/read.c -o src/read.o

# Regra para gerar o arquivo de objeto token.o
src/token.o: src/token.c include/token.h
	@$(CC) $(CFLAGS) -c src/token.c -o src/token.o

# Regra para limpar arquivos gerados
clean:
	@rm -f src/*.o $(TARGET) saida.dat > /dev/null 2>&1

test:
	diff $(OUTPUT_FILE) $(GOLDEN_FILE)