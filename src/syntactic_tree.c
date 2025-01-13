#include <stdio.h>
#include <stdlib.h>
#include "syntactic_tree.h"

SyntacticNode *create_node(NodeType type, Token token) {
    SyntacticNode *node = (SyntacticNode *)malloc(sizeof(SyntacticNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->token = token;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_syntactic_tree(SyntacticNode *root) {
    if (root) {
        free_syntactic_tree(root->left);
        free_syntactic_tree(root->right);
        free(root);
    }
}

void print_syntactic_tree(SyntacticNode *root, int level) {
    if (root) {
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("Node Type: %d, Token Type: %d\n", root->type, root->token.type);
        print_syntactic_tree(root->left, level + 1);
        print_syntactic_tree(root->right, level + 1);
    }
}