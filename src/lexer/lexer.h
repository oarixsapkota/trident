#ifndef TRIDENT_LEXER_H
#define TRIDENT_LEXER_H

#include "token.h"

Token *lexer(const char* input, Lexer **lex_error_head, int *error);

#endif
