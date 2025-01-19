#include "token.h"
#include "lexer.h"
#include "syntactic_tree.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

Token empty_token;

// Declarações das funções
int parse_primitive_types_declaration(SyntacticNode *node, Token *tokens, int *index);
int parse_vector_declaration(SyntacticNode *node, Token *tokens, int *index);
int parse_function_arguments(SyntacticNode *node, Token *tokens, int *index);
int parse_function_declaration(SyntacticNode *node, Token *tokens, int *index);
int parse_assignment(SyntacticNode *node, Token *tokens, int *index);
int parse_function_call(SyntacticNode *node, Token *tokens, int *index);
int parse_if(SyntacticNode *node, Token *tokens, int *index);
int parse_while(SyntacticNode *node, Token *tokens, int *index);
int parse_for(SyntacticNode *node, Token *tokens, int *index);
int parse_eof(SyntacticNode *node, Token *tokens, int *index);
int parse_statement(SyntacticNode *node, Token *tokens, int *index);
void parse_tokens(TokenArray *token_array, SyntacticNode **root);
int parse_expression(SyntacticNode *node, Token *tokens, int *index);
int parse_term(SyntacticNode *node, Token *tokens, int *index);
int parse_factor(SyntacticNode *node, Token *tokens, int *index);
int parse_return(SyntacticNode *node, Token *tokens, int *index);

/*
    FUNÇÕES DE DECLARAÇÃO
*/

int parse_primitive_types_declaration(SyntacticNode *node, Token *tokens, int *index)
{
    int start_pos = *index;
    if (tokens[*index].type == TOKEN_INT ||
        tokens[*index].type == TOKEN_FLOAT ||
        tokens[*index].type == TOKEN_CHAR ||
        tokens[*index].type == TOKEN_STRING)
    {
        node->left = create_node(NODE_PRIMITIVE_TYPE, tokens[*index]);
        (*index)++;
        if (tokens[*index].type == TOKEN_IDENTIFIER)
        {
            node->right = create_node(NODE_LITERAL, tokens[*index]);
            (*index)++;

            if (tokens[*index].type == TOKEN_SEMICOLON)
            {
                node->type = NODE_DECLARATION;
                (*index)++;
                return 1;
            }
            else
            {
                fprintf(stderr, "Erro de sintaxe: esperado ';' na declaração de variável\n");
            }
            free_syntactic_tree(node->right);
            node->right = NULL;
        }
        else
        {
            fprintf(stderr, "Erro de sintaxe: esperado identificador\n");
        }
        free_syntactic_tree(node->left);
        node->left = NULL;
    }
    *index = start_pos;
    return -1;
}

int parse_vector_declaration(SyntacticNode *node, Token *tokens, int *index)
{
    int start_pos = *index;
    if (tokens[*index].type == TOKEN_VECTOR)
    {
        node->type = NODE_VECTOR_DECLARATION;
        node->token = tokens[*index];
        (*index)++;
        if (tokens[*index].type == TOKEN_IDENTIFIER)
        {
            node->left = create_node(NODE_VECTOR_NAME, tokens[*index]);
            (*index)++;
            if (tokens[*index].type == TOKEN_LBRACKET)
            {
                (*index)++;
                if (tokens[*index].type == TOKEN_NUMBER_INT)
                {
                    node->right = create_node(NODE_VECTOR_SIZE, tokens[*index]);
                    (*index)++;
                    if (tokens[*index].type == TOKEN_RBRACKET)
                    {
                        (*index)++;
                        if (tokens[*index].type == TOKEN_SEMICOLON)
                        {
                            (*index)++;
                            return 1;
                        }
                        else
                        {
                            fprintf(stderr, "Erro de sintaxe: esperado ';'\n");
                        }
                    }
                    else
                    {
                        fprintf(stderr, "Erro de sintaxe: esperado ']'\n");
                    }
                    free_syntactic_tree(node->right);
                }
                else
                {
                    fprintf(stderr, "Erro de sintaxe: esperado tamanho do vetor\n");
                }
            }
            else
            {
                fprintf(stderr, "Erro de sintaxe: esperado '['\n");
            }
            free_syntactic_tree(node->left);
        }
        else
        {
            fprintf(stderr, "Erro de sintaxe: esperado identificador\n");
        }
        node->type = NULL_NODE;
        node->token = empty_token;
    }
    *index = start_pos;
    return -1;
}

int parse_function_arguments(SyntacticNode *node, Token *tokens, int *index)
{
    if (tokens[*index].type == TOKEN_INT ||
        tokens[*index].type == TOKEN_FLOAT ||
        tokens[*index].type == TOKEN_CHAR ||
        tokens[*index].type == TOKEN_STRING ||
        tokens[*index].type == TOKEN_VECTOR)
    {
        node->left = create_node(NODE_TYPE, tokens[*index]);
        (*index)++;
        if (tokens[*index].type == TOKEN_IDENTIFIER)
        {
            node->right = create_node(NODE_LITERAL, tokens[*index]);
            (*index)++;
            return 1;
        }
        else
        {
            fprintf(stderr, "Erro de sintaxe: esperado identificador\n");
        }
        free_syntactic_tree(node->left);
        node->left = NULL;
    }
    else
    {
        fprintf(stderr, "Erro de sintaxe: esperado algum tipo\n");
    }
    node->type = NULL_NODE;
    return -1;
}

int parse_return(SyntacticNode *node, Token *tokens, int *index)
{
    int start_pos = *index;
    if (tokens[*index].type == TOKEN_RETURN)
    {
        node->type = NODE_RETURN_STATEMENT;
        (*index)++;
        SyntacticNode *expression = create_node(NODE_EXPRESSION, empty_token);
        node->left = expression;
        if (parse_expression(expression, tokens, index) == 1)
        {
            if (tokens[*index].type == TOKEN_SEMICOLON)
            {
                (*index)++;
                return 1;
            }
            else
            {
                fprintf(stderr, "Erro de sintaxe: esperado ';' após expressão de retorno\n");
            }
        }
        else
        {
            fprintf(stderr, "Erro de sintaxe: esperado expressão após 'return'\n");
        }
        free_syntactic_tree(expression);
        node->left = NULL;
    }
    *index = start_pos;
    return -1;
}

int parse_function_declaration(SyntacticNode *node, Token *tokens, int *index)
{
    int start_pos = *index;
    if (tokens[*index].type == TOKEN_FUNCTION)
    {
        node->type = NODE_FUNCTION_DECLARATION;
        (*index)++;
        if (tokens[*index].type == TOKEN_IDENTIFIER)
        {
            node->left = create_node(NODE_IDENTIFIER, tokens[*index]);
            (*index)++;
            if (tokens[*index].type == TOKEN_LPAREN)
            {
                (*index)++;
                SyntacticNode *function_parameters = create_node(NODE_FUNCTION_PARAMETERS, empty_token);
                node->left->right = function_parameters;
                while (tokens[*index].type != TOKEN_RPAREN)
                {
                    if (parse_function_arguments(function_parameters, tokens, index) == -1)
                    {
                        free_syntactic_tree(function_parameters);
                        return -1;
                    }
                    if (tokens[*index].type == TOKEN_COMMA)
                    {
                        (*index)++;
                        function_parameters->next = create_node(NODE_FUNCTION_PARAMETERS, empty_token);
                        function_parameters = function_parameters->next;
                    }
                }
                if (tokens[*index].type == TOKEN_RPAREN)
                {
                    (*index)++;
                    if (tokens[*index].type == TOKEN_LBRACE)
                    {
                        (*index)++;
                        SyntacticNode *function_body = create_node(NODE_FUNCTION_BODY, empty_token);
                        node->right = function_body;
                        SyntacticNode *statement = create_node(NULL_NODE, empty_token);
                        function_body->left = statement;
                        while (tokens[*index].type != TOKEN_RBRACE)
                        {
                            if (parse_return(statement, tokens, index) == -1 &&
                                parse_assignment(statement, tokens, index) == -1)
                            {
                                free_syntactic_tree(statement);
                                return -1;
                            }
                            if (tokens[*index].type != TOKEN_RBRACE)
                            {
                                statement->next = create_node(NULL_NODE, empty_token);
                                statement = statement->next;
                            }
                        }
                        if (tokens[*index].type == TOKEN_RBRACE)
                        {
                            (*index)++;
                            return 1;
                        }
                        else
                        {
                            fprintf(stderr, "Erro de sintaxe: esperado '}'\n");
                        }
                        free_syntactic_tree(function_body);
                    }
                    else
                    {
                        fprintf(stderr, "Erro de sintaxe: esperado '{' na declaração de função\n");
                    }
                }
                else
                {
                    fprintf(stderr, "Erro de sintaxe: esperado ')' na declaração de função\n");
                }
                free_syntactic_tree(function_parameters);
            }
            else
            {
                fprintf(stderr, "Erro de sintaxe: esperado '(' na declaração de função\n");
            }
            free_syntactic_tree(node->left);
        }
        else
        {
            fprintf(stderr, "Erro de sintaxe: esperado identificador na declaração de função\n");
        }
    }
    *index = start_pos;
    return -1;
}

/*
    FUNÇÕES DE ASSIGNMENT
*/
int parse_assignment(SyntacticNode *node, Token *tokens, int *index)
{
    int start_pos = *index;
    if (tokens[*index].type == TOKEN_IDENTIFIER)
    {
        node->type = NODE_ASSIGNMENT;
        node->left = create_node(NODE_LITERAL, tokens[*index]);
        (*index)++;
        if (tokens[*index].type == TOKEN_ASSIGN)
        {
            (*index)++;
            SyntacticNode *expression = create_node(NODE_EXPRESSION, empty_token);
            if (parse_expression(expression, tokens, index) == 1)
            {
                node->right = expression;
                if (tokens[*index].type == TOKEN_SEMICOLON)
                {
                    (*index)++;
                    return 1;
                }
                else
                {
                    fprintf(stderr, "Erro de sintaxe: esperado ';'\n");
                }
            }
            free_syntactic_tree(expression);
        }
        else
        {
            fprintf(stderr, "Erro de sintaxe: esperado '='\n");
        }
        free_syntactic_tree(node->left);
        node->type = NULL_NODE;
    }
    *index = start_pos;
    return -1;
}

int parse_expression(SyntacticNode *node, Token *tokens, int *index)
{
    SyntacticNode *left = create_node(NODE_TERM, (Token){0});
    if (parse_term(left, tokens, index) == -1)
    {
        free_syntactic_tree(left);
        return -1;
    }

    while (tokens[*index].type == TOKEN_PLUS || tokens[*index].type == TOKEN_MINUS)
    {
        Token op_token = tokens[(*index)++];
        SyntacticNode *right = create_node(NODE_TERM, (Token){0});
        if (parse_term(right, tokens, index) == -1)
        {
            free_syntactic_tree(left);
            free_syntactic_tree(right);
            return -1;
        }

        SyntacticNode *op_node = create_node(NODE_BINARY_OP, op_token);
        op_node->left = left;
        op_node->right = right;
        left = op_node;
    }

    node->type = left->type;
    node->token = left->token;
    node->left = left->left;
    node->right = left->right;
    free(left);

    return 1;
}

int parse_term(SyntacticNode *node, Token *tokens, int *index)
{
    SyntacticNode *left = create_node(NODE_FACTOR, (Token){0});
    if (parse_factor(left, tokens, index) == -1)
    {
        free_syntactic_tree(left);
        return -1;
    }

    while (tokens[*index].type == TOKEN_MULT || tokens[*index].type == TOKEN_DIV)
    {
        Token op_token = tokens[(*index)++];
        SyntacticNode *right = create_node(NODE_FACTOR, (Token){0});
        if (parse_factor(right, tokens, index) == -1)
        {
            free_syntactic_tree(left);
            free_syntactic_tree(right);
            return -1;
        }

        SyntacticNode *op_node = create_node(NODE_BINARY_OP, op_token);
        op_node->left = left;
        op_node->right = right;
        left = op_node;
    }

    node->type = left->type;
    node->token = left->token;
    node->left = left->left;
    node->right = left->right;
    free(left);

    return 1;
}

int parse_factor(SyntacticNode *node, Token *tokens, int *index)
{
    Token current_token = tokens[*index];

    if (current_token.type == TOKEN_LPAREN)
    {
        (*index)++;
        if (parse_expression(node, tokens, index) == -1)
        {
            return -1;
        }

        if (tokens[*index].type != TOKEN_RPAREN)
        {
            fprintf(stderr, "Error: Expected ')' at line %zu\n", tokens[*index].linha);
            return -1;
        }

        (*index)++; // Skip ')'
        return 1;
    }
    else if (current_token.type == TOKEN_NUMBER_INT ||
             current_token.type == TOKEN_NUMBER_FLOAT ||
             current_token.type == TOKEN_IDENTIFIER)
    {
        node->type = NODE_LITERAL;
        node->token = current_token;
        (*index)++;
        return 1;
    }
    else
    {
        fprintf(stderr, "Error: Unexpected token '%d' at line %zu\n", current_token.type, current_token.linha);
        return -1;
    }
}

int parse_function_call_args(SyntacticNode *node, Token *tokens, int *index)
{
    if (tokens[*index].type == TOKEN_NUMBER_INT ||
        tokens[*index].type == TOKEN_NUMBER_FLOAT ||
        tokens[*index].type == TOKEN_CHAR_LITERAL ||
        tokens[*index].type == TOKEN_STRING_LITERAL ||
        tokens[*index].type == TOKEN_IDENTIFIER)
    {
        if (tokens[*index].type == TOKEN_IDENTIFIER)
        {
            node = create_node(NODE_IDENTIFIER, tokens[*index]);
        }
        else
        {
            node = create_node(NODE_LITERAL, tokens[*index]);
        }
        (*index)++;
        return 1;
    }
    return -1;
}

int parse_function_call(SyntacticNode *node, Token *tokens, int *index)
{
    int start_pos = *index;
    if (tokens[*index].type == TOKEN_IDENTIFIER)
    {
        node->type = NODE_FUNCTION_CALL;
        node->left = create_node(NODE_LITERAL, tokens[*index]);
        (*index)++;
        if (tokens[*index].type == TOKEN_LPAREN)
        {
            (*index)++;
            if (tokens[*index].type != TOKEN_RPAREN)
            {
                SyntacticNode *function_arguments = create_node(NODE_FUNCTION_ARGUMENTS, empty_token);
                node->right = function_arguments;
                SyntacticNode *current_argument = create_node(NODE_FUNCTION_ARGUMENTS, empty_token);
                function_arguments->left = current_argument;
                while (tokens[*index].type != TOKEN_RPAREN)
                {
                    if (parse_expression(current_argument, tokens, index) == -1)
                    {
                        free_syntactic_tree(current_argument);
                        break;
                    }
                    if (tokens[*index].type == TOKEN_COMMA)
                    {
                        (*index)++;
                        current_argument->next = create_node(NULL_NODE, empty_token);
                        current_argument = current_argument->next;
                    }
                }
            }
            if (tokens[*index].type == TOKEN_RPAREN)
            {
                (*index)++;
                if (tokens[*index].type == TOKEN_SEMICOLON)
                {
                    (*index)++;
                    return 1;
                }
                else
                {
                    fprintf(stderr, "Erro de sintaxe: esperado ';'\n");
                }
            }
            else
            {
                fprintf(stderr, "Erro de sintaxe: esperado ')'\n");
            }
            free_syntactic_tree(node->left);
        }
        else
        {
            fprintf(stderr, "Erro de sintaxe: esperado '('\n");
        }
        node->type = NULL_NODE;
    }
    *index = start_pos;
    return -1;
}

int parse_print_call(SyntacticNode *node, Token *tokens, int *index)
{
    int start_pos = *index;
    if (tokens[*index].type == TOKEN_PRINT)
    {
        node->type = NODE_PRINT_CALL;
        (*index)++;
        if (tokens[*index].type == TOKEN_LPAREN)
        {
            (*index)++;
            if (tokens[*index].type == TOKEN_STRING_LITERAL)
            {
                node->left = create_node(NODE_LITERAL, tokens[*index]);
                (*index)++;
                if (tokens[*index].type == TOKEN_RPAREN)
                {
                    (*index)++;
                    if (tokens[*index].type == TOKEN_SEMICOLON)
                    {
                        (*index)++;
                        return 1;
                    }
                    else
                    {
                        fprintf(stderr, "Erro de sintaxe: esperado ';'\n");
                    }
                }
                else
                {
                    fprintf(stderr, "Erro de sintaxe: esperado ')'\n");
                }
                free_syntactic_tree(node->left);
            }
            else
            {
                fprintf(stderr, "Erro de sintaxe: esperado string literal\n");
            }
        }
        else
        {
            fprintf(stderr, "Erro de sintaxe: esperado '('\n");
        }
        node->type = NULL_NODE;
    }
    *index = start_pos;
    return -1;
}

int parse_scan_call(SyntacticNode *node, Token *tokens, int *index)
{
    int start_pos = *index;
    if (tokens[*index].type == TOKEN_SCAN)
    {
        node->type = NODE_SCAN_CALL;
        (*index)++;
        if (tokens[*index].type == TOKEN_LPAREN)
        {
            (*index)++;
            if (tokens[*index].type == TOKEN_IDENTIFIER)
            {
                node->left = create_node(NODE_LITERAL, tokens[*index]);
                (*index)++;
                if (tokens[*index].type == TOKEN_RPAREN)
                {
                    (*index)++;
                    if (tokens[*index].type == TOKEN_SEMICOLON)
                    {
                        (*index)++;
                        return 1;
                    }
                    else
                    {
                        fprintf(stderr, "Erro de sintaxe: esperado ';'\n");
                    }
                }
                else
                {
                    fprintf(stderr, "Erro de sintaxe: esperado ')'\n");
                }
                free_syntactic_tree(node->left);
            }
            else
            {
                fprintf(stderr, "Erro de sintaxe: esperado identificador\n");
            }
        }
        else
        {
            fprintf(stderr, "Erro de sintaxe: esperado '('\n");
        }
        node->type = NULL_NODE;
    }
    *index = start_pos;
    return -1;
}

/*
    FUNÇÕES DE CONTROLE DE FLUXO
*/

int parse_comparison(SyntacticNode *node, Token *tokens, int *index)
{
    int start_pos = *index;
    if (tokens[*index].type == TOKEN_NUMBER_INT ||
        tokens[*index].type == TOKEN_NUMBER_FLOAT ||
        tokens[*index].type == TOKEN_CHAR_LITERAL ||
        tokens[*index].type == TOKEN_IDENTIFIER)
    {
        SyntacticNode *operand = create_node(NODE_LITERAL, tokens[*index]);
        (*index)++;

        if (tokens[*index].type == TOKEN_EQ ||
            tokens[*index].type == TOKEN_NEQ ||
            tokens[*index].type == TOKEN_LT ||
            tokens[*index].type == TOKEN_GT ||
            tokens[*index].type == TOKEN_LTE ||
            tokens[*index].type == TOKEN_GTE)
        {
            node->type = NODE_BINARY_OP;
            node->token = tokens[*index];
            (*index)++;

            if (tokens[*index].type == TOKEN_NUMBER_INT ||
                tokens[*index].type == TOKEN_NUMBER_FLOAT ||
                tokens[*index].type == TOKEN_CHAR_LITERAL ||
                tokens[*index].type == TOKEN_IDENTIFIER)
            {
                node->left = operand;
                node->right = create_node(NODE_LITERAL, tokens[*index]);
                (*index)++;
                return 1;
            }
            fprintf(stderr, "Erro de sintaxe: esperado operando INT, FLOAT, CHAR OU VARIÁVEL\n");
            node->type = NULL_NODE;
            node->token = empty_token;
        }
        else
        {
            fprintf(stderr, "Erro de sintaxe: esperado operador de comparação\n");
        }
        free_syntactic_tree(operand);
    }
    else
    {
        fprintf(stderr, "Erro de sintaxe: esperado operando INT, FLOAT, CHAR OU VARIÁVEL\n");
    }
    *index = start_pos;
    return -1;
}

int parse_else(SyntacticNode *node, Token *tokens, int *index)
{
    int start_pos = *index;
    if (tokens[*index].type == TOKEN_ELSE)
    {
        node->type = NODE_ELSE_BODY;
        (*index)++;
        if (tokens[*index].type == TOKEN_LBRACE)
        {
            (*index)++;
            SyntacticNode *statement = create_node(NULL_NODE, empty_token);
            node->left = statement;
            while (tokens[*index].type != TOKEN_RBRACE)
            {
                if (parse_statement(statement, tokens, index) == -1)
                {
                    free_syntactic_tree(statement);
                    break;
                }
                if (tokens[*index].type != TOKEN_RBRACE)
                {
                    statement->next = create_node(NULL_NODE, empty_token);
                    statement = statement->next;
                }
            }
            if (tokens[*index].type == TOKEN_RBRACE)
            {
                (*index)++;
                return 1;
            }
            else
            {
                fprintf(stderr, "Erro de sintaxe: esperado '}'\n");
            }
            free_syntactic_tree(statement);
            node->left = NULL;
        }
        else
        {
            fprintf(stderr, "Erro de sintaxe: esperado '{'\n");
        }
        node->type = NULL_NODE;
    }
    *index = start_pos;
    return -1;
}

int parse_if(SyntacticNode *node, Token *tokens, int *index)
{
    int start_pos = *index;
    if (tokens[*index].type == TOKEN_IF)
    {
        node->type = NODE_IF_STATEMENT;
        (*index)++;
        if (tokens[*index].type == TOKEN_LPAREN)
        {
            SyntacticNode *condition = create_node(NULL_NODE, empty_token);
            (*index)++;
            if (parse_comparison(condition, tokens, index) == 1)
            {
                if (tokens[*index].type == TOKEN_RPAREN)
                {
                    (*index)++;
                    if (tokens[*index].type == TOKEN_LBRACE)
                    {
                        (*index)++;
                        SyntacticNode *if_body = create_node(NODE_IF_BODY, empty_token);
                        node->left = condition;
                        node->right = if_body;
                        SyntacticNode *statement = create_node(NULL_NODE, empty_token);
                        if_body->left = statement;
                        while (tokens[*index].type != TOKEN_RBRACE)
                        {
                            if (parse_statement(statement, tokens, index) == -1)
                            {
                                free_syntactic_tree(statement);
                                break;
                            }
                            if (tokens[*index].type != TOKEN_RBRACE)
                            {
                                statement->next = create_node(NULL_NODE, empty_token);
                                statement = statement->next;
                            }
                        }
                        if (tokens[*index].type == TOKEN_RBRACE)
                        {
                            (*index)++;
                            if (tokens[*index].type == TOKEN_ELSE)
                            {
                                SyntacticNode *else_node = create_node(NODE_ELSE_STATEMENT, tokens[*index]);
                                if (parse_else(else_node, tokens, index) == 1)
                                {
                                    if_body->right = else_node;
                                    return 1;
                                }
                                free_syntactic_tree(else_node);
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            fprintf(stderr, "Erro de sintaxe: esperado '}'\n");
                        }
                        free_syntactic_tree(statement);
                        free_syntactic_tree(if_body);
                    }
                    else
                    {
                        fprintf(stderr, "Erro de sintaxe: esperado '{'\n");
                    }
                }
                else
                {
                    fprintf(stderr, "Erro de sintaxe: esperado ')'\n");
                }
            }
            free_syntactic_tree(condition);
        }
        else
        {
            fprintf(stderr, "Erro de sintaxe: esperado '('\n");
        }
    }
    *index = start_pos;
    return -1;
}

int parse_while(SyntacticNode *node, Token *tokens, int *index)
{
    int start_pos = *index;
    if (tokens[*index].type == TOKEN_WHILE)
    {
        node->type = NODE_WHILE_STATEMENT;
        (*index)++;
        if (tokens[*index].type == TOKEN_LPAREN)
        {
            SyntacticNode *condition = create_node(NULL_NODE, empty_token);
            (*index)++;
            if (parse_comparison(condition, tokens, index) == 1)
            {
                if (tokens[*index].type == TOKEN_RPAREN)
                {
                    (*index)++;
                    if (tokens[*index].type == TOKEN_LBRACE)
                    {
                        (*index)++;
                        SyntacticNode *while_body = create_node(NODE_WHILE_BODY, empty_token);
                        node->left = condition;
                        node->right = while_body;
                        SyntacticNode *statement = create_node(NULL_NODE, empty_token);
                        while_body->left = statement;
                        while (tokens[*index].type != TOKEN_RBRACE)
                        {
                            if (parse_statement(statement, tokens, index) == -1)
                            {
                                free_syntactic_tree(statement);
                                break;
                            }
                            statement->next = create_node(NULL_NODE, empty_token);
                            statement = statement->next;
                        }
                        if (tokens[*index].type == TOKEN_RBRACE)
                        {
                            (*index)++;
                            return 1;
                        }
                        else
                        {
                            fprintf(stderr, "Erro de sintaxe: esperado '}'\n");
                        }
                        free_syntactic_tree(statement);
                        free_syntactic_tree(while_body);
                    }
                    else
                    {
                        fprintf(stderr, "Erro de sintaxe: esperado '{'\n");
                    }
                }
                else
                {
                    fprintf(stderr, "Erro de sintaxe: esperado ')'\n");
                }
            }
            free_syntactic_tree(condition);
        }
        else
        {
            fprintf(stderr, "Erro de sintaxe: esperado '('\n");
        }
    }
    *index = start_pos;
    return -1;
}

int parse_for_condition(SyntacticNode *node, Token *tokens, int *index)
{
    if (tokens[*index].type == TOKEN_IDENTIFIER)
    {
        node->type = NODE_FOR_CONDITION;
        SyntacticNode *for_operand = create_node(NODE_LITERAL, tokens[*index]);
        node->left = for_operand;
        (*index)++;
        if (tokens[*index].type == TOKEN_EQ ||
            tokens[*index].type == TOKEN_NEQ ||
            tokens[*index].type == TOKEN_LT ||
            tokens[*index].type == TOKEN_GT ||
            tokens[*index].type == TOKEN_LTE ||
            tokens[*index].type == TOKEN_GTE)
        {
            node->token = tokens[*index];
            (*index)++;
            if (tokens[*index].type == TOKEN_NUMBER_INT ||
                tokens[*index].type == TOKEN_IDENTIFIER)
            {
                node->right = create_node(NODE_LITERAL, tokens[*index]);
                (*index)++;
                return 1;
            }
            else
            {
                fprintf(stderr, "Erro de sintaxe: esperado operando int ou identificador\n");
            }
        }
        else
        {
            fprintf(stderr, "Erro de sintaxe: esperado operador de comparação\n");
        }
        free_syntactic_tree(node->left);
    }
    else
    {
        fprintf(stderr, "Erro de sintaxe: esperado identificador\n");
    }
    return -1;
}

int parse_for(SyntacticNode *node, Token *tokens, int *index)
{
    int start_pos = *index;
    if (tokens[*index].type == TOKEN_FOR)
    {
        (*index)++;
        node->type = NODE_FOR_STATEMENT;
        if (tokens[*index].type == TOKEN_LPAREN)
        {
            (*index)++;
            if (tokens[*index].type == TOKEN_IDENTIFIER)
            {
                SyntacticNode *for_elements = create_node(NODE_FOR_ELEMENTS, empty_token);
                node->left = for_elements;
                for_elements->left = create_node(NODE_LITERAL, tokens[*index]);
                (*index)++;
                if (tokens[*index].type == TOKEN_SEMICOLON)
                {
                    SyntacticNode *for_cond_incr = create_node(NODE_FOR_COND_INC, empty_token);
                    for_elements->right = for_cond_incr;
                    SyntacticNode *for_condition = create_node(NODE_FOR_CONDITION, empty_token);
                    for_cond_incr->left = for_condition;
                    (*index)++;
                    if (parse_for_condition(for_condition, tokens, index) == 1)
                    {
                        if (tokens[*index].type == TOKEN_SEMICOLON)
                        {
                            SyntacticNode *for_increment = create_node(NODE_FOR_INCREMENT, empty_token);
                            for_cond_incr->right = for_increment;
                            SyntacticNode *for_increment_statement = create_node(NULL_NODE, empty_token);
                            for_increment->left = for_increment_statement;
                            (*index)++;
                            if (parse_assignment(for_increment_statement, tokens, index) == 1)
                            {
                                if (tokens[*index].type == TOKEN_RPAREN)
                                {
                                    (*index)++;
                                    if (tokens[*index].type == TOKEN_LBRACE)
                                    {
                                        (*index)++;
                                        SyntacticNode *for_body = create_node(NODE_FOR_BODY, empty_token);
                                        node->right = for_body;
                                        SyntacticNode *statement = create_node(NULL_NODE, empty_token);
                                        for_body->left = statement;
                                        while (tokens[*index].type != TOKEN_RBRACE)
                                        {
                                            if (parse_statement(statement, tokens, index) == -1)
                                            {
                                                free_syntactic_tree(statement);
                                                break;
                                            }
                                            if (tokens[*index].type != TOKEN_RBRACE)
                                            {
                                                statement->next = create_node(NULL_NODE, empty_token);
                                                statement = statement->next;
                                            }
                                        }
                                        if (tokens[*index].type == TOKEN_RBRACE)
                                        {
                                            (*index)++;
                                            return 1;
                                        }
                                        else
                                        {
                                            fprintf(stderr, "Erro de sintaxe: esperado '}'\n");
                                        }
                                        free_syntactic_tree(statement);
                                        free_syntactic_tree(for_body);
                                    }
                                    else
                                    {
                                        fprintf(stderr, "Erro de sintaxe: esperado '{'\n");
                                    }
                                }
                                else
                                {
                                    fprintf(stderr, "Erro de sintaxe: esperado ')'\n");
                                }
                            }
                        }
                        else
                        {
                            fprintf(stderr, "Erro de sintaxe: esperado ';' 2\n");
                        }
                    }
                }
                else
                {
                    fprintf(stderr, "Erro de sintaxe: esperado ';' 1\n");
                }
                free_syntactic_tree(node->left);
            }
            else
            {
                fprintf(stderr, "Erro de sintaxe: esperado identificador para variável de controle do for\n");
            }
        }
        else
        {
            fprintf(stderr, "Erro de sintaxe: esperado '(' na declaração de for\n");
        }
    }
    *index = start_pos;
    node->type = NULL_NODE;
    return -1;
}

int parse_eof(SyntacticNode *node, Token *tokens, int *index)
{
    if (tokens[*index].type == TOKEN_EOF)
    {
        node->type = NODE_EOF;
        (*index)++;
        return 1;
    }
    return -1;
}

int parse_statement(SyntacticNode *node, Token *tokens, int *index)
{
    int result = 0;
    switch (tokens[*index].type)
    {
    case TOKEN_INT:
    case TOKEN_FLOAT:
    case TOKEN_CHAR:
    case TOKEN_STRING:
        result = parse_primitive_types_declaration(node, tokens, index);
        break;
    case TOKEN_VECTOR:
        result = parse_vector_declaration(node, tokens, index);
        break;
    case TOKEN_FUNCTION:
        result = parse_function_declaration(node, tokens, index);
        break;
    case TOKEN_IDENTIFIER:
        result = parse_assignment(node, tokens, index);
        if (result == -1)
        {
            result = parse_function_call(node, tokens, index);
        }
        break;
    case TOKEN_PRINT:
        result = parse_print_call(node, tokens, index);
        break;
    case TOKEN_SCAN:
        result = parse_scan_call(node, tokens, index);
        break;
    case TOKEN_IF:
        result = parse_if(node, tokens, index);
        break;
    case TOKEN_WHILE:
        result = parse_while(node, tokens, index);
        break;
    case TOKEN_FOR:
        result = parse_for(node, tokens, index);
        break;
    case TOKEN_EOF:
        result = parse_eof(node, tokens, index);
        break;
    default:
        result = -1;
        break;
    }

    if (result == -1)
    {
        return -1;
    }

    return 1;
}

void parse_tokens(TokenArray *token_array, SyntacticNode **root)
{
    *root = create_node(NODE_PROGRAM, empty_token);
    SyntacticNode *current_statement = create_node(NULL_NODE, empty_token);
    (*root)->next = current_statement;
    int index = 0;

    while (index < token_array->size)
    {
        if (parse_statement(current_statement, token_array->tokens, &index) == -1)
        {
            fprintf(stderr, "Erro de sintaxe: instrução inválida\n");
            free_syntactic_tree(*root);
            exit(EXIT_FAILURE);
        }
        if (index < token_array->size)
        {
            current_statement->next = create_node(NULL_NODE, empty_token);
            current_statement = current_statement->next;
        }
    }
}