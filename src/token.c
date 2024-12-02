#include <stdio.h>
#include <stdlib.h>

#include "token.h"

void init_token_array(TokenArray *arr) {
    arr->size = 0;
    arr->capacity = 10;
    arr->tokens = (Token *)malloc(arr->capacity * sizeof(Token));
    if (arr->tokens == NULL) {
        // Falha na alocação de memória
        fprintf(stderr, "Erro de alocação de memória para tokens\n");
        exit(EXIT_FAILURE);
    }
}

void add_token(TokenArray *arr, Token token) {
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        Token *new_tokens = (Token *)realloc(arr->tokens, arr->capacity * sizeof(Token));
        if (new_tokens == NULL) {
            // Falha na realocação de memória
            fprintf(stderr, "Erro de realocação de memória para tokens\n");
            free(arr->tokens);
            exit(EXIT_FAILURE);
        }
        arr->tokens = new_tokens;
    }
    arr->tokens[arr->size++] = token;
}

void free_token_array(TokenArray *arr) {
    // Libera `str_value` apenas para os tokens que armazenam strings dinâmicas
    for (size_t i = 0; i < arr->size; i++) {
        if (arr->tokens[i].type == TOKEN_IDENTIFIER || 
            arr->tokens[i].type == TOKEN_STRING_LITERAL || 
            arr->tokens[i].type == TOKEN_CHAR_LITERAL) {
            free(arr->tokens[i].str_value); // Libera a string alocada dinamicamente
        }
    }

    // Libera o array de tokens
    free(arr->tokens);
    arr->tokens = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

void print_token_array(const TokenArray *arr) {
    for (size_t i = 0; i < arr->size; i++) {
        Token token = arr->tokens[i];
        printf("Token %ld: ", i);
        switch (token.type) {
            case TOKEN_EOF:
                printf("EOF\n");
                break;
             case TOKEN_NUMBER_INT:
                printf("Integer: %lld\n", token.int_value);
                break;
            case TOKEN_NUMBER_FLOAT:
                printf("Float: %Lf\n", token.float_value);
                break;
            case TOKEN_IDENTIFIER:
                printf("Identifier: %s\n", token.str_value);
                break;
            case TOKEN_STRING:
                printf("String: %s\n", token.str_value);
                break;
            case TOKEN_CHAR_LITERAL:
                printf("Char Literal: %s\n", token.str_value);
                break;
            case TOKEN_STRING_LITERAL:
                printf("String Literal: %s\n", token.str_value);
                break;
            case TOKEN_INT:
                printf("Keyword: int\n");
                break;
            case TOKEN_FLOAT:
                printf("Keyword: float\n");
                break;
            case TOKEN_CHAR:
                printf("Keyword: char\n");
                break;
            case TOKEN_IF:
                printf("Keyword: if\n");
                break;
            case TOKEN_ELSE:
                printf("Keyword: else\n");
                break;
            case TOKEN_WHILE:
                printf("Keyword: while\n");
                break;
            case TOKEN_FOR:
                printf("Keyword: for\n");
                break;
            case TOKEN_RETURN:
                printf("Keyword: return\n");
                break;
            case TOKEN_BREAK:
                printf("Keyword: break\n");
                break;
            case TOKEN_FUNCTION:
                printf("Keyword: function\n");
                break;
            case TOKEN_PRINT:
                printf("Keyword: print\n");
                break;
            case TOKEN_SCAN:
                printf("Keyword: read\n");
                break;
            case TOKEN_PLUS:
                printf("Operator: +\n");
                break;
            case TOKEN_MINUS:
                printf("Operator: -\n");
                break;
            case TOKEN_MULT:
                printf("Operator: *\n");
                break;
            case TOKEN_DIV:
                printf("Operator: /\n");
                break;
            case TOKEN_NOT:
                printf("Operator: !\n");
                break;
            case TOKEN_ASSIGN:
                printf("Operator: =\n");
                break;
            case TOKEN_EQ:
                printf("Operator: ==\n");
                break;
            case TOKEN_NEQ:
                printf("Operator: !=\n");
                break;
            case TOKEN_LT:
                printf("Operator: <\n");
                break;
            case TOKEN_GT:
                printf("Operator: >\n");
                break;
            case TOKEN_LTE:
                printf("Operator: <=\n");
                break;
            case TOKEN_GTE:
                printf("Operator: >=\n");
                break;
            case TOKEN_LPAREN:
                printf("Delimiter: (\n");
                break;
            case TOKEN_RPAREN:
                printf("Delimiter: )\n");
                break;
            case TOKEN_LBRACE:
                printf("Delimiter: {\n");
                break;
            case TOKEN_RBRACE:
                printf("Delimiter: }\n");
                break;
            case TOKEN_LBRACKET:
                printf("Delimiter: [\n");
                break;
            case TOKEN_RBRACKET:
                printf("Delimiter: ]\n");
                break;
            case TOKEN_SEMICOLON:
                printf("Delimiter: ;\n");
                break;
            case TOKEN_COMMA:
                printf("Delimiter: ,\n");
                break;
            default:
                printf("Unknown token type\n");
                break;
        }
    }
}

