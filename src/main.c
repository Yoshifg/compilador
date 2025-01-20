#include <stdio.h>
#include <stdlib.h>

#include "file_utils.h"
#include "lexer.h"
#include "token.h"
#include "syntactic_tree.h"
#include "parser.h"
#include "semantic_analysis.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
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

    SyntacticNode *root = NULL;
    parse_tokens(&token_array, &root);
    print_tree(root);

    SymbolTable table;
    init_symbol_table(&table);
    printf("Iniciando análise semântica...\n");
    semantic_analysis(root, &table);
    printf("Análise semântica concluída!\n");


    free(source);
    return EXIT_SUCCESS;
}