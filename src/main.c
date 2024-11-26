#include <stdio.h>
#include <stdlib.h>

#include "file_utils.h"
#include "lexer.h"
#include "token.h"

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

    print_token_array(&token_array);
    free_token_array(&token_array);

    free(source);
    return EXIT_SUCCESS;
}