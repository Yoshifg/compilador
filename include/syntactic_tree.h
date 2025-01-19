#ifndef SYNTACTIC_TREE_H
#define SYNTACTIC_TREE_H

#include "token.h"

typedef enum {
    NODE_PROGRAM,
    NODE_DECLARATION,
    NODE_TYPE,
    NODE_PRIMITIVE_TYPE,
    NODE_VECTOR_DECLARATION,
    NODE_VECTOR_TYPE,
    NODE_VECTOR_DESCRIPTION,
    NODE_VECTOR_NAME,
    NODE_VECTOR_SIZE,
    NODE_FUNCTION_DECLARATION,
    NODE_FUNCTION_PARAMETERS,
    NODE_FUNCTION_BODY,
    NODE_EXPRESSION,
    NODE_TERM,
    NODE_FACTOR,
    NODE_LITERAL,
    NODE_IDENTIFIER,
    NODE_ASSIGNMENT,
    NODE_BINARY_OP,
    NODE_FUNCTION_CALL,
    NODE_FUNCTION_ARGUMENTS,
    NODE_PRINT_CALL,
    NODE_SCAN_CALL,
    NODE_IF_STATEMENT,
    NODE_ELSE_STATEMENT,
    NODE_IF_BODY,
    NODE_ELSE_BODY,
    NODE_WHILE_STATEMENT,
    NODE_WHILE_BODY,
    NODE_FOR_STATEMENT,
    NODE_FOR_ELEMENTS,
    NODE_FOR_COND_INC,
    NODE_FOR_CONDITION,
    NODE_FOR_INCREMENT,
    NODE_FOR_BODY,
    NODE_RETURN_STATEMENT,
    NODE_EOF,
    NULL_NODE,
} NodeType;

typedef struct SyntacticNode {
    NodeType type;
    Token token;
    struct SyntacticNode *left;
    struct SyntacticNode *right;
    struct SyntacticNode *next; // Para listas de declarações ou instruções
} SyntacticNode;

SyntacticNode *create_node(NodeType type, Token token);
void free_syntactic_tree(SyntacticNode *root);
const char *node_type_to_string(NodeType type);
void print_tree(SyntacticNode *root);

#endif // SYNTACTIC_TREE_H