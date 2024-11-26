#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

#include "token.h"

typedef struct {
    const char *source;
    size_t length;
    size_t position;
    int line;
} Lexer;

void lexer_init(Lexer *lexer, const char *source);
Token make_token(Lexer *lexer, TokenType type);
char *strndup(const char *src, size_t n);
Token read_number(Lexer *lexer);
Token read_identifier_or_keyword(Lexer *lexer);
Token read_string_literal(Lexer *lexer);
Token read_operators_delimiters(Lexer *lexer);
void tokenize(Lexer *lexer, TokenArray *token_array);

#endif // LEXER_H