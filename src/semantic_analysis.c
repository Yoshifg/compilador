#include "syntactic_tree.h"
#include "symbol_table.h"
#include "semantic_analysis.h"
#include <stdio.h>

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
    enter_scope();
    SyntacticNode *param = node->left->right;
    while (param)
    {
        char *param_name = param->right->token.str_value;
        if (find_symbol(table, param_name))
        {
            fprintf(stderr, "Erro: Parâmetro '%s' já declarado\n", param_name);
        }
        else
        {
            add_symbol(table, param_name, SYMBOL_INT); // Exemplo, mapear conforme necessário
        }
        param = param->next;
    }
    semantic_analysis(node->right->left, table);
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

void analyze_for_statement(SyntacticNode, SymbolTable *table)
{
    
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
    case NODE_IF_STATEMENT:
        analyze_if_statement(node, table);
        break;
    case NODE_WHILE_STATEMENT:
        analyze_while_statement(node, table);
        break;
    // Outros casos...
    default:
        break;
    }

    // Análise recursiva
    semantic_analysis(node->left, table);
    semantic_analysis(node->right, table);
    semantic_analysis(node->next, table);
}