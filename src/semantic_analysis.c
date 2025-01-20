#include "syntactic_tree.h"
#include "symbol_table.h"
#include "semantic_analysis.h"
#include <stdio.h>
#include <stdlib.h>

void analyze_declaration(SyntacticNode *node, SymbolTable *table)
{
    char *name = node->right->token.str_value;
    SymbolType type = SYMBOL_VOID;
    switch (node->left->type)
    {
    case NODE_PRIMITIVE_TYPE:
        switch (node->left->token.type)
        {
        case TOKEN_INT:
            type = SYMBOL_INT;
            break;
        case TOKEN_FLOAT:
            type = SYMBOL_FLOAT;
            break;
        case TOKEN_CHAR:
            type = SYMBOL_CHAR;
            break;
        case TOKEN_STRING:
            type = SYMBOL_STRING;
            break;
        default:
            type = SYMBOL_VOID;
            break;
        }
        break;
    default:
        fprintf(stderr, "Erro: Tipo de declaração não reconhecido\n");
        type = SYMBOL_VOID;
        break;
    }
    if (find_symbol(table, name))
    {
        fprintf(stderr, "Erro: Identificador '%s' já declarado no escopo atual\n", name);
    }
    else
    {
        add_symbol(table, name, type);
    }
}

void analyze_vector_declaration(SyntacticNode *node, SymbolTable *table)
{
    char *name = node->left->right->token.str_value;
    if (find_symbol(table, name))
    {
        fprintf(stderr, "Erro: Identificador '%s' já declarado no escopo atual\n", name);
    }
    else
    {
        add_symbol(table, name, SYMBOL_VECTOR);
    }
}

void analyze_function_declaration(SyntacticNode *node, SymbolTable *table)
{
    char *name = node->left->token.str_value;

    if (find_symbol(table, name))
    {
        fprintf(stderr, "Erro: Função '%s' já declarada\n", name);
    }
    else
    {
        add_symbol(table, name, SYMBOL_FUNCTION);
    }

    Symbol *func_symbol = find_symbol(table, name);
    if (!func_symbol)
    {
        return;
    }
    size_t capacity = 8;
    func_symbol->param_types = (SymbolType *)malloc(capacity * sizeof(SymbolType));
    func_symbol->param_count = 0;

    // Entramos no escopo da função
    enter_scope();
    SyntacticNode *param = node->left->right;
    while (param)
    {
        SymbolType ptype = SYMBOL_INT;
        switch (param->left->token.type)
        {
        case TOKEN_INT:
            ptype = SYMBOL_INT;
            break;
        case TOKEN_FLOAT:
            ptype = SYMBOL_FLOAT;
            break;
        case TOKEN_CHAR:
            ptype = SYMBOL_CHAR;
            break;
        case TOKEN_STRING:
            ptype = SYMBOL_STRING;
            break;
        default:
            ptype = SYMBOL_VOID;
            break;
        }

        if (func_symbol->param_count >= capacity)
        {
            capacity *= 2;
            func_symbol->param_types = (SymbolType *)realloc(func_symbol->param_types, capacity * sizeof(SymbolType));
        }
        func_symbol->param_types[func_symbol->param_count] = ptype;
        func_symbol->param_count++;

        char *param_name = param->right->token.str_value;
        if (find_symbol(table, param_name))
        {
            fprintf(stderr, "Erro: Parâmetro '%s' já declarado\n", param_name);
        }
        else
        {
            add_symbol(table, param_name, ptype);
        }
        param = param->next;
    }
    semantic_analysis(node->right, table);
    leave_scope(table);
}

void analyze_assignment(SyntacticNode *node, SymbolTable *table)
{
    char *name = node->left->token.str_value;
    Symbol *symbol = find_symbol(table, name);
    if (!symbol)
    {
        fprintf(stderr, "Erro: Identificador '%s' não declarado\n", name);
    }
    else
    {
        SyntacticNode *expression = node->right;
        if (symbol->type == SYMBOL_CHAR)
        {
            if (expression->token.type == TOKEN_IDENTIFIER)
            {
                Symbol *expression_symbol = find_symbol(table, expression->token.str_value);
                if (!expression_symbol)
                {
                    fprintf(stderr, "Erro: Identificador '%s' não declarado\n", expression->token.str_value);
                }
                else if (expression_symbol->type != SYMBOL_CHAR)
                {
                    fprintf(stderr, "Erro: Atribuição de tipos incompatíveis\n");
                }
            }
            else if (expression->token.type != TOKEN_CHAR_LITERAL)
            {
                fprintf(stderr, "Erro: Atribuição de tipos incompatíveis\n");
            }
        }
        else if (symbol->type == SYMBOL_STRING)
        {
            if (expression->token.type == TOKEN_IDENTIFIER)
            {
                Symbol *expression_symbol = find_symbol(table, expression->token.str_value);
                if (!expression_symbol)
                {
                    fprintf(stderr, "Erro: Identificador '%s' não declarado\n", expression->token.str_value);
                }
                else if (expression_symbol->type != SYMBOL_STRING)
                {
                    fprintf(stderr, "Erro: Atribuição de tipos incompatíveis\n");
                }
            }
            else if (expression->token.type != TOKEN_STRING_LITERAL)
            {
                fprintf(stderr, "Erro: Atribuição de tipos incompatíveis\n");
            }
        }
        else if (symbol->type == SYMBOL_VECTOR ||
                 symbol->type == SYMBOL_INT ||
                 symbol->type == SYMBOL_FLOAT)
        {
            if (expression->token.type == TOKEN_IDENTIFIER)
            {
                Symbol *expression_symbol = find_symbol(table, expression->token.str_value);
                if (!expression_symbol)
                {
                    fprintf(stderr, "Erro: Identificador '%s' não declarado\n", expression->token.str_value);
                }
                else if (expression_symbol->type != SYMBOL_INT && expression_symbol->type != SYMBOL_FLOAT)
                {
                    fprintf(stderr, "Erro: Atribuição de tipos incompatíveis\n");
                }
            }
            else if (expression->type == NODE_BINARY_OP)
            {
                while (expression->right->type == NODE_BINARY_OP)
                {
                    if (expression->left->token.type == TOKEN_IDENTIFIER)
                    {
                        Symbol *left_symbol = find_symbol(table, expression->left->token.str_value);
                        if (!left_symbol)
                        {
                            fprintf(stderr, "Erro: Identificador '%s' não declarado\n", expression->left->token.str_value);
                        }
                        else if (left_symbol->type != SYMBOL_INT && left_symbol->type != SYMBOL_FLOAT)
                        {
                            fprintf(stderr, "Erro: Atribuição de tipos incompatíveis\n");
                        }
                    }
                    if (expression->right->token.type == TOKEN_IDENTIFIER)
                    {
                        Symbol *right_symbol = find_symbol(table, expression->right->token.str_value);
                        if (!right_symbol)
                        {
                            fprintf(stderr, "Erro: Identificador '%s' não declarado\n", expression->right->token.str_value);
                        }
                        else if (right_symbol->type != SYMBOL_INT && right_symbol->type != SYMBOL_FLOAT)
                        {
                            fprintf(stderr, "Erro: Atribuição de tipos incompatíveis\n");
                        }
                    }
                    expression = expression->right;
                }
            }
            else if (expression->type == NODE_LITERAL)
            {
                if (expression->token.type != TOKEN_NUMBER_INT && expression->token.type != TOKEN_NUMBER_FLOAT)
                {
                    fprintf(stderr, "Erro: Atribuição de tipos incompatíveis\n");
                }
            }
        }
        else
        {
            fprintf(stderr, "Erro: Atribuição para tipo inválido\n");
        }
    }
}

void analyze_function_call(SyntacticNode *node, SymbolTable *table)
{
    // node->left: Identificador da função
    // node->right: Lista de argumentos
    char *func_name = node->left->token.str_value;
    Symbol *func_symbol = find_symbol(table, func_name);
    if (!func_symbol)
    {
        fprintf(stderr, "Erro: Função '%s' não declarada\n", func_name);
        return;
    }
    if (func_symbol->type != SYMBOL_FUNCTION)
    {
        fprintf(stderr, "Erro: '%s' não é uma função\n", func_name);
        return;
    }

    // Exemplo: supondo que a symbol_table armazene quantos parâmetros a função possui
    // e seus tipos em um array func_symbol->param_types[] e em func_symbol->param_count

    // Percorre os nós de argumentos e compara com a lista de tipos esperados
    SyntacticNode *arg_node = node->right->left;   // Início da lista de argumentos
    int i = 0;
    while (arg_node && i < func_symbol->param_count)
    {
        // Descobre o tipo do argumento, por exemplo, usando a mesma lógica de analyze_assignment
        SymbolType arg_type = SYMBOL_VOID;
        if (arg_node->type == NODE_LITERAL)
        {
            switch (arg_node->token.type)
            {
            case TOKEN_NUMBER_INT:
                arg_type = SYMBOL_INT;
                break;
            case TOKEN_NUMBER_FLOAT:
                arg_type = SYMBOL_FLOAT;
                break;
            case TOKEN_CHAR_LITERAL:
                arg_type = SYMBOL_CHAR;
                break;
            case TOKEN_STRING_LITERAL:
                arg_type = SYMBOL_STRING;
                break;
            default:
                break;
            }
        }
        else if (arg_node->type == NODE_IDENTIFIER)
        {
            Symbol *s = find_symbol(table, arg_node->token.str_value);
            if (s)
            {
                arg_type = s->type;
            }
            else
            {
                fprintf(stderr, "Erro: Identificador '%s' não declarado\n", arg_node->token.str_value);
            }
        }
        // ...ou mais lógica para expressões, etc.

        // Compara com o tipo esperado para esse parâmetro
        if (arg_type != func_symbol->param_types[i])
        {
            fprintf(stderr, "Erro: Tipo do argumento %d incompatível em '%s'\n", i + 1, func_name);
        }

        arg_node = arg_node->next; // Próximo argumento
        i++;
    }

    // Verifica se o número de argumentos passados confere com o número esperado
    if (i != func_symbol->param_count || arg_node != NULL)
    {
        fprintf(stderr, "Erro: Quantidade de argumentos incompatível na chamada de '%s'\n", func_name);
    }
}

void analyse_comparison_types(SyntacticNode *node, SymbolTable *table)
{
    if (node->left->type == NODE_BINARY_OP)
    {
        analyse_comparison_types(node->left, table);
    }
    if (node->right->type == NODE_BINARY_OP)
    {
        analyse_comparison_types(node->right, table);
    }
    if (node->left->token.type == TOKEN_IDENTIFIER)
    {
        Symbol *left_symbol = find_symbol(table, node->left->token.str_value);
        if (!left_symbol)
        {
            fprintf(stderr, "Erro: Identificador '%s' não declarado\n", node->left->token.str_value);
        }
        else if (left_symbol->type != SYMBOL_INT && left_symbol->type != SYMBOL_FLOAT)
        {
            fprintf(stderr, "Erro: Comparação de tipos incompatíveis\n");
        }
    }
    else if (node->left->token.type != TOKEN_NUMBER_INT && node->left->token.type != TOKEN_NUMBER_FLOAT)
    {
        fprintf(stderr, "Erro: Comparação de tipos incompatíveis\n");
    }
    if (node->right->token.type == TOKEN_IDENTIFIER)
    {
        Symbol *right_symbol = find_symbol(table, node->right->token.str_value);
        if (!right_symbol)
        {
            fprintf(stderr, "Erro: Identificador '%s' não declarado\n", node->right->token.str_value);
        }
        else if (right_symbol->type != SYMBOL_INT && right_symbol->type != SYMBOL_FLOAT)
        {
            fprintf(stderr, "Erro: Comparação de tipos incompatíveis\n");
        }
    }
    else if (node->right->token.type != TOKEN_NUMBER_INT && node->right->token.type != TOKEN_NUMBER_FLOAT)
    {
        fprintf(stderr, "Erro: Comparação de tipos incompatíveis\n");
    }
}

void analyze_if_statement(SyntacticNode *node, SymbolTable *table)
{
    if (node->left->type != NODE_BINARY_OP)
    {
        fprintf(stderr, "Erro: Condição do 'if' deve ser booleana\n");
    }
    else
    {
        if (node->left->type == NODE_BINARY_OP)
        {
            analyse_comparison_types(node->left, table);
        }
    }
    enter_scope();
    semantic_analysis(node->right->left, table); // Corpo do 'if'
    leave_scope(table);
}

void analyze_while_statement(SyntacticNode *node, SymbolTable *table)
{
    if (node->left->type != NODE_BINARY_OP)
    {
        fprintf(stderr, "Erro: Condição do 'while' deve ser booleana\n");
    }
    else
    {
        if (node->left->type == NODE_BINARY_OP)
        {
            analyse_comparison_types(node->left, table);
        }
    }
    enter_scope();
    semantic_analysis(node->right->left, table); // Corpo do 'while'
    leave_scope(table);
}

void analyze_for_statement(SyntacticNode *node, SymbolTable *table)
{
    char *name = node->left->left->token.str_value;
    Symbol *symbol = find_symbol(table, name);
    if (!symbol)
    {
        fprintf(stderr, "Erro: Identificador '%s' não declarado\n", name);
    }
    else if (symbol->type != SYMBOL_INT)
    {
        fprintf(stderr, "Erro: Identificador '%s' deve ser do tipo INT\n", name);
    }

    if (node->left->right->left->type == NODE_FOR_COND_INC)
    {
        analyse_comparison_types(node->left->right->left, table);
    }

    if (node->left->right->right->type == NODE_FOR_INCREMENT)
    {
        if (node->left->right->right->token.type == TOKEN_IDENTIFIER)
        {
            analyze_assignment(node->left->right->right->left, table);
        }
    }

    enter_scope();
    semantic_analysis(node->right->left, table); // Corpo do 'for'
    leave_scope(table);
}

void semantic_analysis(SyntacticNode *node, SymbolTable *table)
{
    if (!node)
        return;

    switch (node->type)
    {
    case NODE_DECLARATION:
        analyze_declaration(node, table);
        break;
    case NODE_VECTOR_DECLARATION:
        analyze_vector_declaration(node, table);
        break;
    case NODE_FUNCTION_DECLARATION:
        analyze_function_declaration(node, table);
        break;
    case NODE_ASSIGNMENT:
        analyze_assignment(node, table);
        break;
    case NODE_FUNCTION_CALL:
        analyze_function_call(node, table);
        break;
    case NODE_IF_STATEMENT:
        analyze_if_statement(node, table);
        break;
    case NODE_WHILE_STATEMENT:
        analyze_while_statement(node, table);
        break;
    case NODE_FOR_STATEMENT:
        analyze_for_statement(node, table);
        break;
    default:
        break;
    }

    // Análise recursiva
    semantic_analysis(node->left, table);
    semantic_analysis(node->right, table);
    semantic_analysis(node->next, table);
}