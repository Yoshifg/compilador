#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>

typedef enum
{
    TOKEN_EOF,            // Fim do arquivo
    TOKEN_NUM,            // Token numérico
    TOKEN_ID,             // Token identificador
    TOKEN_INT,            // Palavra-chave inteiro
    TOKEN_FLOAT,          // Palavra-chave float
    TOKEN_CHAR,           // Palavra-chave char
    TOKEN_STRING,         // Palavra-chave string
    TOKEN_CHAR_LITERAL,   // Literal de caractere
    TOKEN_STRING_LITERAL, // Literal de string
    TOKEN_IF,             // Palavra-chave if
    TOKEN_ELSE,           // Palavra-chave else
    TOKEN_WHILE,          // Palavra-chave while
    TOKEN_FOR,            // Palavra-chave for
    TOKEN_RETURN,         // Palavra-chave return
    TOKEN_BREAK,          // Palavra-chave break
    TOKEN_FUNCTION,       // Palavra-chave function
    TOKEN_PLUS,           // Operador de adição
    TOKEN_MINUS,          // Operador de subtração
    TOKEN_MULT,           // Operador de multiplicação
    TOKEN_DIV,            // Operador de divisão
    TOKEN_ASSIGN,         // Operador de atribuição
    TOKEN_EQ,             // Operador de igualdade
    TOKEN_NEQ,            // Operador de diferença
    TOKEN_LT,             // Operador menor que
    TOKEN_GT,             // Operador maior que
    TOKEN_LTE,            // Operador menor ou igual
    TOKEN_GTE,            // Operador maior ou igual
    TOKEN_LPAREN,         // Parêntese esquerdo
    TOKEN_RPAREN,         // Parêntese direito
    TOKEN_LBRACE,         // Chave esquerda
    TOKEN_RBRACE,         // Chave direita
    TOKEN_LBRACKET,       // Colchete esquerdo
    TOKEN_RBRACKET,       // Colchete direito
    TOKEN_SEMICOLON,      // Ponto e vírgula
    TOKEN_COMMA           // Vírgula
} TokenType;

typedef struct
{
    TokenType type;
    size_t linha;
    union
    {
        long long int int_value; // valor numérico
        long double float_value;
        char *str_value;         // para strings literais e identificadores
    };
} Token;

typedef struct
{
    Token* tokens;
    size_t size;
    size_t capacity;
} TokenArray;

TokenArray* init_token_array();

void add_token(TokenArray* array, Token token);

void free_token_array(TokenArray* array);

void print_token_array(TokenArray* array);

#endif // TOKEN_H
