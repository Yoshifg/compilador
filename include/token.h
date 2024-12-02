#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

typedef enum
{
    // Fim do arquivo
    TOKEN_EOF,

    // Literais
    TOKEN_NUMBER_INT,            // Token numérico
    TOKEN_NUMBER_FLOAT,          // Token numérico
    TOKEN_IDENTIFIER,        // Token identificador
    TOKEN_STRING,            // Token string
    TOKEN_CHAR_LITERAL,      // Literal de caractere
    TOKEN_STRING_LITERAL,    // Literal de string

    // Palavras-chave (Keyword)
    TOKEN_INT,               // Palavra-chave inteiro
    TOKEN_FLOAT,             // Palavra-chave float
    TOKEN_CHAR,              // Palavra-chave char
    TOKEN_IF,                // Palavra-chave if
    TOKEN_ELSE,              // Palavra-chave else
    TOKEN_WHILE,             // Palavra-chave while
    TOKEN_FOR,               // Palavra-chave for
    TOKEN_RETURN,            // Palavra-chave return
    TOKEN_BREAK,             // Palavra-chave break
    TOKEN_FUNCTION,          // Palavra-chave function
    TOKEN_PRINT,             // Palavra-chave print
    TOKEN_SCAN,              // Palavra-chave read
    TOKEN_VECTOR,

    // Operadores
    TOKEN_PLUS,              // Operador de adição + 
    TOKEN_MINUS,             // Operador de subtração -
    TOKEN_MULT,              // Operador de multiplicação *
    TOKEN_DIV,               // Operador de divisão / 
    TOKEN_NOT ,              // Operador de negação !
    TOKEN_ASSIGN,            // Operador de atribuição = 
    TOKEN_EQ,                // Operador de igualdade == 
    TOKEN_NEQ,               // Operador de diferença != 
    TOKEN_LT,                // Operador menor que <
    TOKEN_GT,                // Operador maior que > 
    TOKEN_LTE,               // Operador menor ou igual <= 
    TOKEN_GTE,               // Operador maior ou igual >=

    // Outros simbolos
    TOKEN_LPAREN,            // Parêntese esquerdo (
    TOKEN_RPAREN,            // Parêntese direito )
    TOKEN_LBRACE,            // Chave esquerda {
    TOKEN_RBRACE,            // Chave direita }
    TOKEN_LBRACKET,          // Colchete esquerdo [
    TOKEN_RBRACKET,          // Colchete direito ]
    TOKEN_SEMICOLON,         // Ponto e vírgula ; DELIMITADOR
    TOKEN_COMMA,            // Vírgula ,
} TokenType;

typedef struct {
    TokenType type;
    size_t linha;
    long long int int_value;
    long double float_value;
    char *str_value;  // Identificadores ou strings literais ou nome_vetor
    long int vector_size; // Tamanho do vetor
} Token;

typedef struct {
    Token *tokens;    // Array dinâmico para armazenar tokens
    size_t size;      // Quantidade atual de tokens armazenados
    size_t capacity;  // Capacidade máxima antes de precisar redimensionar
} TokenArray;

void init_token_array(TokenArray *arr);
void add_token(TokenArray *arr, Token token);
void free_token_array(TokenArray *arr);
void print_token_array(const TokenArray *arr);

#endif // TOKEN_H
