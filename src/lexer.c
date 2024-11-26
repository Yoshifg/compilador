#include "lexer.h"
#include "token.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Inicializa o lexer com a fonte de entrada
void lexer_init(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->length = strlen(source);
    lexer->position = 0;
    lexer->line = 1;
}

// Cria um token com o tipo especificado
Token make_token(Lexer *lexer, TokenType type) {
    Token token;
    token.type = type;
    token.linha = lexer->line;
    token.int_value = 0;
    token.float_value = 0.0;
    token.str_value = NULL;
    return token;
}

// Duplica uma string com um tamanho específico
char *strndup(const char *src, size_t n) {
    char *dest = (char *)malloc(n + 1);
    if (dest) {
        strncpy(dest, src, n);
        dest[n] = '\0';
    }
    return dest;
}

// Lê um número da entrada e cria um token de número
Token read_number(Lexer *lexer) {
    Token token;
    const char *start = &lexer->source[lexer->position];
    int is_float = 0;
    while (lexer->position < lexer->length && (isdigit(lexer->source[lexer->position]) || lexer->source[lexer->position] == '.')) {
        if (lexer->source[lexer->position] == '.') {
            is_float = 1;
        }
        lexer->position++;
    }
    size_t length = lexer->position - (start - lexer->source);
    char *number_str = strndup(start, length);

    if (is_float) {
        token = make_token(lexer, TOKEN_NUMBER_FLOAT);
        token.float_value = atof(number_str);
    } else {
        token = make_token(lexer, TOKEN_NUMBER_INT);
        token.int_value = atoll(number_str);
    }
    free(number_str);
    return token;
}

// Lê um identificador ou palavra-chave da entrada e cria um token correspondente
Token read_identifier_or_keyword(Lexer *lexer) {
    const char *start = &lexer->source[lexer->position];
    size_t length = 0;
    while (lexer->position < lexer->length && (isalnum(lexer->source[lexer->position]) || lexer->source[lexer->position] == '_')) {
        lexer->position++;
        length++;
    }
    Token token = make_token(lexer, TOKEN_IDENTIFIER);
    token.str_value = strndup(start, length);

    // Verifica se o identificador é uma palavra-chave
    if (strcmp(token.str_value, "int") == 0) {
        token.type = TOKEN_INT;
    } else if (strcmp(token.str_value, "float") == 0) {
        token.type = TOKEN_FLOAT;
    } else if (strcmp(token.str_value, "char") == 0) {
        token.type = TOKEN_CHAR;
    } else if (strcmp(token.str_value, "if") == 0) {
        token.type = TOKEN_IF;
    } else if (strcmp(token.str_value, "else") == 0) {
        token.type = TOKEN_ELSE;
    } else if (strcmp(token.str_value, "while") == 0) {
        token.type = TOKEN_WHILE;
    } else if (strcmp(token.str_value, "for") == 0) {
        token.type = TOKEN_FOR;
    } else if (strcmp(token.str_value, "return") == 0) {
        token.type = TOKEN_RETURN;
    } else if (strcmp(token.str_value, "break") == 0) {
        token.type = TOKEN_BREAK;
    } else if (strcmp(token.str_value, "function") == 0) {
        token.type = TOKEN_FUNCTION;
    }

    return token;
}

// Lê uma string literal da entrada e cria um token de string
Token read_string_literal(Lexer *lexer) {
    lexer->position++; // Pula a aspa inicial
    const char *start = &lexer->source[lexer->position];
    while (lexer->position < lexer->length && lexer->source[lexer->position] != '"') {
        lexer->position++;
    }
    size_t length = lexer->position - (start - lexer->source);
    char *string_str = strndup(start, length);
    Token token = make_token(lexer, TOKEN_STRING_LITERAL);
    token.str_value = string_str;
    lexer->position++; // Pula a aspa final
    return token;
}

// Lê um caractere literal da entrada e cria um token de caractere
Token read_char_literal(Lexer *lexer) {
    lexer->position++; // Pula a aspa simples inicial
    char char_value = lexer->source[lexer->position];
    lexer->position++; // Pula o caractere
    lexer->position++; // Pula a aspa simples final
    Token token = make_token(lexer, TOKEN_CHAR_LITERAL);
    char *string_str = strndup(&char_value, 1);
    token.str_value = string_str;
    return token;
}

// Lê operadores e delimitadores da entrada e cria um token correspondente
Token read_operators_delimiters(Lexer *lexer) {
    Token token;
    char current_char = lexer->source[lexer->position];

    switch (current_char) {
        case '+':
            token = make_token(lexer, TOKEN_PLUS);
            break;
        case '-':
            token = make_token(lexer, TOKEN_MINUS);
            break;
        case '*':
            token = make_token(lexer, TOKEN_MULT);
            break;
        case '/':
            token = make_token(lexer, TOKEN_DIV);
            break;
        case '!':
            if (lexer->source[lexer->position + 1] == '=') {
                token = make_token(lexer, TOKEN_NEQ);
                lexer->position++;
            } else {
                token = make_token(lexer, TOKEN_NOT);
            }
            break;
        case '=':
            if (lexer->source[lexer->position + 1] == '=') {
                token = make_token(lexer, TOKEN_EQ);
                lexer->position++;
            } else {
                token = make_token(lexer, TOKEN_ASSIGN);
            }
            break;
        case '<':
            if (lexer->source[lexer->position + 1] == '=') {
                token = make_token(lexer, TOKEN_LTE);
                lexer->position++;
            } else {
                token = make_token(lexer, TOKEN_LT);
            }
            break;
        case '>':
            if (lexer->source[lexer->position + 1] == '=') {
                token = make_token(lexer, TOKEN_GTE);
                lexer->position++;
            } else {
                token = make_token(lexer, TOKEN_GT);
            }
            break;
        case '(':
            token = make_token(lexer, TOKEN_LPAREN);
            break;
        case ')':
            token = make_token(lexer, TOKEN_RPAREN);
            break;
        case '{':
            token = make_token(lexer, TOKEN_LBRACE);
            break;
        case '}':
            token = make_token(lexer, TOKEN_RBRACE);
            break;
        case '[':
            token = make_token(lexer, TOKEN_LBRACKET);
            break;
        case ']':
            token = make_token(lexer, TOKEN_RBRACKET);
            break;
        case ';':
            token = make_token(lexer, TOKEN_SEMICOLON);
            break;
        case ',':
            token = make_token(lexer, TOKEN_COMMA);
            break;
        default:
            token = make_token(lexer, TOKEN_EOF);
            break;
    }

    lexer->position++;
    return token;
}

// Ignora comentários na entrada
void skip_comment(Lexer *lexer) {
    while (lexer->position < lexer->length && lexer->source[lexer->position] != '\n') {
        lexer->position++;
    }
    // Pula o caractere de nova linha também
    if (lexer->position < lexer->length && lexer->source[lexer->position] == '\n') {
        lexer->position++;
    }
    lexer->line++;
}

// Tokeniza a entrada e armazena os tokens no array de tokens
void tokenize(Lexer *lexer, TokenArray *token_array) {
    initTokenArray(token_array);

    while (lexer->position < lexer->length) {
        char current_char = lexer->source[lexer->position];

        if (isspace(current_char)) {
            if (current_char == '\n') {
                lexer->line++;
            }
            lexer->position++;
            continue;
        }

        if (current_char == '#') {
            skip_comment(lexer);
            continue;
        }

        Token token;
        if (isdigit(current_char)) {
            token = read_number(lexer);
        } else if (isalpha(current_char) || current_char == '_') {
            token = read_identifier_or_keyword(lexer);
        } else if (current_char == '"') {
            token = read_string_literal(lexer);
        } else if (current_char == '\'') {
            token = read_char_literal(lexer);
        } else {
            token = read_operators_delimiters(lexer);
        }

        addToken(token_array, token);
    }

    // Adiciona um token EOF no final
    Token eof_token = make_token(lexer, TOKEN_EOF);
    addToken(token_array, eof_token);
}