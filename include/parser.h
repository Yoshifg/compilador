#ifndef PARSER_H

#include "syntactic_tree.h"
#include "token.h"

void parse_tokens(TokenArray *token_array, SyntacticNode **root);

#endif // PARSER_H