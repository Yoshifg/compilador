#include <stdio.h>
#include "token.h"

TokenArray *init_token_array()
{
    TokenArray *array = (TokenArray *)malloc(sizeof(TokenArray));
    if (!array)
    {
        fprintf(stderr, "Failed to allocate memory for TokenArray structure\n");
        exit(EXIT_FAILURE);
    }

    array->size = 0;
    array->capacity = 1;
    array->tokens = (Token *)malloc(sizeof(Token) * array->capacity);

    if (!array->tokens)
    {
        fprintf(stderr, "Failed to allocate memory for tokens array\n");
        free(array); // Libera a memória alocada para a estrutura TokenArray
        exit(EXIT_FAILURE);
    }

    return array;
}

void add_token(TokenArray *array, Token token)
{
    if (array->size == array->capacity)
    {
        array->capacity *= 2;
        Token *new_tokens = (Token *)realloc(array->tokens, sizeof(Token) * array->capacity);
        if (!new_tokens)
        {
            fprintf(stderr, "Failed to reallocate memory for tokens array: %s\n");
            free(array->tokens);
            free(array);
            exit(EXIT_FAILURE);
        }
        array->tokens = new_tokens;
    }

    array->tokens[array->size++] = token;
}

void free_token_array(TokenArray *array)
{
    free(array->tokens);
    free(array);
}

void print_token_array(TokenArray *array)
{
    for (size_t i = 0; i < array->size; i++)
    {
        Token token = array->tokens[i];
        switch (token.type)
        {
        case TOKEN_EOF:
            printf("\n");
            break;
        case TOKEN_NUM:
            printf("%lld ", token.int_value);
            break;
        case TOKEN_FLOAT:
            printf("%llf", token.float_value);
        case TOKEN_ID:
        case TOKEN_CHAR:
        case TOKEN_CHAR_LITERAL:
        case TOKEN_STRING:
        case TOKEN_STRING_LITERAL:
            printf("%s ", token.str_value);
            break;
        case TOKEN_IF:
            printf("if ");
            break;
        case TOKEN_ELSE:
            printf("else ");
            break;
        case TOKEN_WHILE:
            printf("while ");
            break;
        case TOKEN_FOR:
            printf("for ");
            break;
        case TOKEN_RETURN:
            printf("return ");
            break;
        case TOKEN_BREAK:
            printf("break ");
            break;
        case TOKEN_FUNCTION:
            printf("function ");
            break;
        case TOKEN_PLUS:
            printf("+ ");
            break;
        case TOKEN_MINUS:
            printf("- ");
            break;
        case TOKEN_MULT:
            printf("* ");
            break;
        case TOKEN_DIV:
            printf("/ ");
            break;
        case TOKEN_ASSIGN:
            printf("= ");
            break;
        case TOKEN_EQ:
            printf("== ");
            break;
        case TOKEN_NEQ:
            printf("!= ");
            break;
        case TOKEN_LT:
            printf("< ");
            break;
        case TOKEN_GT:
            printf("> ");
            break;
        case TOKEN_LTE:
            printf("<= ");
            break;
        case TOKEN_GTE:
            printf(">= ");
            break;
        case TOKEN_LPAREN:
            printf("( ");
            break;
        case TOKEN_RPAREN:
            printf(") ");
            break;
        case TOKEN_LBRACE:
            printf("{ ");
            break;
        case TOKEN_RBRACE:
            printf("} ");
            break;
        case TOKEN_LBRACKET:
            printf("[ ");
            break;
        case TOKEN_RBRACKET:
            printf("] ");
            break;
        case TOKEN_SEMICOLON:
            printf(";\n");
            break;
        case TOKEN_COMMA:
            printf(", ");
            break;
        default:
            fprintf(stderr, "Unknown token type: %d\n", token.type);
            exit(EXIT_FAILURE);
        }
        fflush(stdout);
    }
}