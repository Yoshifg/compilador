CC = gcc
CFLAGS = -Wall -O2 -Iinclude

# Arquivos objeto
OBJS = src/main.o src/lexer.o src/token.o

# Nome do alvo final
TARGET = main

all: $(TARGET)

# Regra principal para gerar o executável
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

# Regra para gerar objeto de src/main.c, com dependências para os respectivos .h
src/main.o: src/main.c include/lexer.h include/token.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

# Regra para gerar objetos de src/lexer.c, src/token.c, com dependências para os respectivos .h
src/lexer.o: src/lexer.c include/lexer.h include/token.h
	$(CC) $(CFLAGS) -c src/lexer.c -o src/lexer.o

src/token.o: src/token.c include/token.h
	$(CC) $(CFLAGS) -c src/token.c -o src/token.o

teste_1: $(TARGET)
	./$(TARGET) tests/lexer_tests/test1.txt

teste_2: $(TARGET)
	./$(TARGET) tests/lexer_tests/test2.txt

teste_3: $(TARGET)
	./$(TARGET) tests/lexer_tests/test3.txt

teste_4: $(TARGET)
	./$(TARGET) tests/lexer_tests/test4.txt

teste_5: $(TARGET)
	./$(TARGET) tests/lexer_tests/test5.txt
	
# Limpeza dos arquivos gerados
clean:
	rm -f $(OBJS) $(TARGET)
