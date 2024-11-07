# Nome do executável
TARGET = lexer

# Compilador e flags
CC = gcc
CFLAGS = -Wall

# Arquivos fonte e de cabeçalho
SRCS = main.c lexer.c token.c
OBJS = $(SRCS:.c=.o)
HEADERS = lexer.h token.h

# Alvo principal
all: $(TARGET)

# Regra para gerar o executável
$(TARGET): $(OBJS)
    $(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regra para gerar os arquivos de objeto
%.o: %.c $(HEADERS)
    $(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar arquivos gerados
clean:
    rm -f $(OBJS) $(TARGET)

# Regra para executar o programa
run: $(TARGET)
    ./$(TARGET) teste.txt