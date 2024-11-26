#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "token.h"

// Função para ler o conteúdo de um arquivo e retornar como uma string
char *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ./lexer <filename>\n");
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    char *source = read_file(filename);

    Lexer lexer;
    lexer_init(&lexer, source);

    TokenArray token_array;
    tokenize(&lexer, &token_array);

    printTokenArray(&token_array);
    free_token_array(&token_array);

    free(source);
    return EXIT_SUCCESS;
}