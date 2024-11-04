#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "token.h"

#define BUFFER_SIZE 1024

typedef struct
{
    FILE *file;
    char buffer[BUFFER_SIZE];
    size_t buffer_pos;
    size_t buffer_size;
} Lexer;

void init_lexer(Lexer *lexer, FILE *file);
int get_next_char(Lexer *lexer);
void unget_char(Lexer *lexer);
int skip_whitespace(Lexer *lexer);
void read_identifier(Lexer *lexer, char *buffer, size_t size);
void read_number(Lexer *lexer, char *buffer, size_t size);
void read_string_literal(Lexer *lexer, char *buffer, size_t size);
Token get_next_token(Lexer *lexer);

#endif // LEXER_H