#ifndef SYNTACTIC_TREE_H
#define SYNTACTIC_TREE_H

#include "token.h"

typedef enum {
    NODE_PROGRAM,
    NODE_STATEMENT,
    NODE_DECLARATION,
    LEAF_IDENTIFIER,
} NodeType;

typedef struct SyntacticNode {
    NodeType type;
    Token token;
    struct SyntacticNode *left;
    struct SyntacticNode *right;
} SyntacticNode;

SyntacticNode *create_node(NodeType type, Token token);
void free_syntactic_tree(SyntacticNode *root);
void print_syntactic_tree(SyntacticNode *root, int level);

#endif // SYNTACTIC_TREE_H