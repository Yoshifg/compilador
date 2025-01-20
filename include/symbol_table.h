#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "syntactic_tree.h"
#include <stddef.h>

// Tipos necessários para a tabela de símbolos
typedef enum
{
    SYMBOL_INT,
    SYMBOL_FLOAT,
    SYMBOL_CHAR,
    SYMBOL_STRING,
    SYMBOL_FUNCTION,
    SYMBOL_VECTOR,
    SYMBOL_VOID
} SymbolType;

typedef struct
{
    char *name;      // Nome do identificador
    SymbolType type; // Tipo do identificador
    size_t scope;    // Nível do escopo
} Symbol;

typedef struct
{
    Symbol *symbols; // Array dinâmico de símbolos
    size_t size;     // Quantidade atual de símbolos
    size_t capacity; // Capacidade máxima antes de redimensionar
} SymbolTable;

// Funções da tabela de símbolos
void init_symbol_table(SymbolTable *table);
void free_symbol_table(SymbolTable *table);
void enter_scope();
void leave_scope(SymbolTable *table);
void add_symbol(SymbolTable *table, char *name, SymbolType type);
Symbol *find_symbol(SymbolTable *table, const char *name);

#endif // SYMBOL_TABLE_H