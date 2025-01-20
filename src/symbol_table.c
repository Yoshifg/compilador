#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symbol_table.h"
#include "syntactic_tree.h"

static size_t current_scope = 0;

void init_symbol_table(SymbolTable *table)
{
    table->symbols = malloc(16 * sizeof(Symbol));
    table->size = 0;
    table->capacity = 16;
}

void free_symbol_table(SymbolTable *table)
{
    for (size_t i = 0; i < table->size; i++)
    {
        free(table->symbols[i].name);
    }
    free(table->symbols);
}

void enter_scope()
{
    current_scope++;
}

void leave_scope(SymbolTable *table)
{
    for (size_t i = 0; i < table->size;)
    {
        if (table->symbols[i].scope == current_scope)
        {
            free(table->symbols[i].name);
            table->symbols[i] = table->symbols[--table->size];
        }
        else
        {
            i++;
        }
    }
    current_scope--;
}

void add_symbol(SymbolTable *table, char *name, SymbolType type)
{
    if (table->size >= table->capacity)
    {
        table->capacity *= 2;
        table->symbols = realloc(table->symbols, table->capacity * sizeof(Symbol));
    }
    table->symbols[table->size++] = (Symbol){
        .name = strdup(name),
        .type = type,
        .scope = current_scope};
}

Symbol *find_symbol(SymbolTable *table, const char *name)
{
    for (size_t i = 0; i < table->size; i++)
    {
        if (strcmp(table->symbols[i].name, name) == 0 && table->symbols[i].scope <= current_scope)
        {
            return &table->symbols[i];
        }
    }
    return NULL;
}