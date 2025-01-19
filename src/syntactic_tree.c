#include <stdio.h>
#include <stdlib.h>
#include "syntactic_tree.h"
#include "token.h"

SyntacticNode *create_node(NodeType type, Token token)
{
    SyntacticNode *node = (SyntacticNode *)malloc(sizeof(SyntacticNode));
    if (!node)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->token = token;
    node->left = NULL;
    node->right = NULL;
    node->next = NULL;
    return node;
}

void free_syntactic_tree(SyntacticNode *root)
{
    if (root)
    {
        free_syntactic_tree(root->left);
        free_syntactic_tree(root->right);
        free_syntactic_tree(root->next);
        free(root);
        root = NULL;
    }
}

// Helper function to get a string representation of a NodeType
const char *node_type_to_string(NodeType type)
{
    switch (type)
    {
    case NODE_PROGRAM:
        return "NODE_PROGRAM";
    case NODE_DECLARATION:
        return "NODE_DECLARATION";
    case NODE_TYPE:
        return "NODE_TYPE";
    case NODE_PRIMITIVE_TYPE:
        return "NODE_PRIMITIVE_TYPE";
    case NODE_VECTOR_DECLARATION:
        return "NODE_VECTOR_DECLARATION";
    case NODE_VECTOR_TYPE:
        return "NODE_VECTOR_TYPE";
    case NODE_VECTOR_DESCRIPTION:
        return "NODE_VECTOR_DESCRIPTION";
    case NODE_VECTOR_NAME:
        return "NODE_VECTOR_NAME";
    case NODE_VECTOR_SIZE:
        return "NODE_VECTOR_SIZE";
    case NODE_FUNCTION_DECLARATION:
        return "NODE_FUNCTION_DECLARATION";
    case NODE_FUNCTION_PARAMETERS:
        return "NODE_FUNCTION_PARAMETERS";
    case NODE_FUNCTION_BODY:
        return "NODE_FUNCTION_BODY";
    case NODE_EXPRESSION:
        return "NODE_EXPRESSION";
    case NODE_TERM:
        return "NODE_TERM";
    case NODE_FACTOR:
        return "NODE_FACTOR";
    case NODE_LITERAL:
        return "NODE_LITERAL";
    case NODE_IDENTIFIER:
        return "NODE_IDENTIFIER";
    case NODE_ASSIGNMENT:
        return "NODE_ASSIGNMENT";
    case NODE_BINARY_OP:
        return "NODE_BINARY_OP";
    case NODE_FUNCTION_CALL:
        return "NODE_FUNCTION_CALL";
    case NODE_FUNCTION_ARGUMENTS:
        return "NODE_FUNCTION_ARGUMENTS";
    case NODE_PRINT_CALL:
        return "NODE_PRINT_CALL";
    case NODE_SCAN_CALL:
        return "NODE_SCAN_CALL";
    case NODE_IF_STATEMENT:
        return "NODE_IF_STATEMENT";
    case NODE_ELSE_STATEMENT:
        return "NODE_ELSE_STATEMENT";
    case NODE_IF_BODY:
        return "NODE_IF_BODY";
    case NODE_ELSE_BODY:
        return "NODE_ELSE_BODY";
    case NODE_WHILE_STATEMENT:
        return "NODE_WHILE_STATEMENT";
    case NODE_FOR_STATEMENT:
        return "NODE_FOR_STATEMENT";
    case NODE_FOR_ELEMENTS:
        return "NODE_FOR_ELEMENTS";
    case NODE_FOR_COND_INC:
        return "NODE_FOR_COND_INC";
    case NODE_FOR_CONDITION:
        return "NODE_FOR_CONDITION";
    case NODE_FOR_INCREMENT:
        return "NODE_FOR_INCREMENT";
    case NODE_FOR_BODY: 
        return "NODE_FOR_BODY";
    case NODE_WHILE_BODY:
        return "NODE_WHILE_BODY";
    case NODE_RETURN_STATEMENT:
        return "NODE_RETURN_STATEMENT";
    case NODE_EOF:
        return "NODE_EOF";
    case NULL_NODE:
        return "NULL_NODE";
    default:
        return "UNKNOWN_NODE_TYPE";
    }
}

// Function to print a single node
void print_node(SyntacticNode *node)
{
    if (node == NULL)
    {
        printf("Nó nulo");
        return;
    }

    printf("=====================================\n");
    printf("Node Type: %s\n", node_type_to_string(node->type));
    printf("Token Type: ");
    print_token(node->token);
    printf("Left Type: %s\n", node->left ? node_type_to_string(node->left->type) : "NULL");
    printf("Right Type: %s\n", node->right ? node_type_to_string(node->right->type) : "NULL");
    printf("Next Type: %s\n", node->next ? node_type_to_string(node->next->type) : "NULL");
}

void print_tree(SyntacticNode *root)
{
    if (root == NULL)
    {
        return;
    }
    else
    {
        print_node(root);
    }

    if (root->left != NULL)
    {
        print_tree(root->left);
    }
    if (root->right != NULL)
    {
        print_tree(root->right);
    }
    if (root->next != NULL)
    {
        print_tree(root->next);
    }
}