#include "token.h"
#include "lexer.h"
#include "syntactic_tree.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

Token empty_token = {0};

int parse_variable_declaration(SyntacticNode *node, Token *tokens, int pos)
{
    if (tokens[pos].type == TOKEN_INT || tokens[pos].type == TOKEN_FLOAT || tokens[pos].type == TOKEN_CHAR)
    {
        SyntacticNode *statemenent = create_node(NODE_STATEMENT, empty_token);
        node->left = statemenent;
        SyntacticNode *declaration = create_node(NODE_DECLARATION, empty_token);
        statemenent->left = declaration;
        declaration->left = create_node(LEAF_IDENTIFIER, tokens[pos]);
        pos++;
    }
    else
    {
        return -1;
    }

    if (tokens[pos].type == TOKEN_IDENTIFIER)
    {
        node->right = create_node(LEAF_IDENTIFIER, tokens[pos]);
        pos++;
    }
    else
    {
        fprintf(stderr, "Erro de sintaxe: esperado identificador\n");
        exit(EXIT_FAILURE);
    }

    if (tokens[pos].type == TOKEN_SEMICOLON)
    {
        return pos++;
    }
    else
    {
        fprintf(stderr, "Erro de sintaxe: esperado ';'\n");
        exit(EXIT_FAILURE);
    }
}

int parse_function_declaration(SyntacticNode *node, Token *tokens, int pos);

int parse_expression(SyntacticNode *node, Token *tokens, int pos);

int parse_assignment(SyntacticNode *node, Token *tokens, int pos)
{
    if (tokens[pos].type == TOKEN_IDENTIFIER)
    {
        SyntacticNode *statemenent = create_node(NODE_STATEMENT, empty_token);
        node->left = statemenent;
        SyntacticNode *assignment = create_node(NODE_DECLARATION, empty_token);
        statemenent->left = assignment;
        assignment->left = create_node(LEAF_IDENTIFIER, tokens[pos]);
        pos++;
    }
    else
    {
        return -1;
    }
}

int parse_statement(SyntacticNode *node, Token *tokens, int pos);

int parse_comparison(SyntacticNode *node, Token *tokens, int pos);

int parse_if(SyntacticNode *node, Token *tokens, int pos); /*
{
    // Cria um nó para a expressão do if
    SyntacticNode *expression = create_node(NODE_EXPRESSION, tokens[pos]);
    node->left = expression;
    pos++;

    if (tokens[pos].type != TOKEN_LPAREN)
    {
        fprintf(stderr, "Erro de sintaxe: esperado '('\n");
        exit(EXIT_FAILURE);
    }
    pos++;

    if (parse_comparison(expression, tokens, pos) == -1)
    {
        fprintf(stderr, "Erro de sintaxe: expressão inválida\n");
        exit(EXIT_FAILURE);
    }

    // Cria um nó para o bloco de código do if
    SyntacticNode *statement = create_node(NODE_STATEMENT, tokens[pos]);
    node->right = statement;
    pos++;

    // Verifica se há um else
    if (tokens[pos].type == TOKEN_ELSE)
    {
        // Cria um nó para o bloco de código do else
        SyntacticNode *else_statement = create_node(NODE_STATEMENT, tokens[pos + 1]);
        statement->right = else_statement;
        pos += 2;
    }

    return pos;
}*/

int parse_while(SyntacticNode *node, Token *tokens, int pos);

int parse_for(SyntacticNode *node, Token *tokens, int pos);