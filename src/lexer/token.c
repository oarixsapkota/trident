#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

Token *add_token(const TokenType type, char *value, const uint64_t line,
                 const uint64_t col) {
  Token *token = malloc(sizeof(Token));
  if (token == NULL)
    return NULL;
  token->type = type;
  token->line = line;
  token->col = col;
  token->value = value;
  token->next = NULL;
  return token;
}

Lexer *add_lexer_error(char *error, const uint64_t line, const uint64_t col,
                       const uint64_t len) {
  Lexer *lexer = malloc(sizeof(Lexer));
  if (lexer == NULL)
    return NULL;
  lexer->msg = error;
  lexer->line = line;
  lexer->col = col;
  lexer->next = NULL;
  lexer->len = len;
  return lexer;
}

TokenType get_keyword_type(const char *word) {
  // Function & Visibility
  if (strcmp(word, "pub") == 0)
    return TOKEN_PUB;
  if (strcmp(word, "func") == 0)
    return TOKEN_FUNC;
  if (strcmp(word, "ret") == 0)
    return TOKEN_RETURN;

  // Control Flow
  if (strcmp(word, "if") == 0)
    return TOKEN_IF;
  if (strcmp(word, "else") == 0)
    return TOKEN_ELSE;
  if (strcmp(word, "while") == 0)
    return TOKEN_WHILE;
  if (strcmp(word, "for") == 0)
    return TOKEN_FOR;
  if (strcmp(word, "loop") == 0)
    return TOKEN_LOOP;
  if (strcmp(word, "break") == 0)
    return TOKEN_BREAK;
  if (strcmp(word, "continue") == 0)
    return TOKEN_CONTINUE;
  if (strcmp(word, "guard") == 0)
    return TOKEN_GUARD;

  // Error Handling
  if (strcmp(word, "try") == 0)
    return TOKEN_TRY;
  if (strcmp(word, "catch") == 0)
    return TOKEN_CATCH;

  // Determiners
  if (strcmp(word, "var") == 0)
    return TOKEN_VAR;
  if (strcmp(word, "const") == 0)
    return TOKEN_CONST;
  if (strcmp(word, "volatile") == 0)
    return TOKEN_VOLATILE;

  // Literals
  if (strcmp(word, "true") == 0)
    return LITERAL_TRUE;
  if (strcmp(word, "false") == 0)
    return LITERAL_FALSE;

  // Base Types
  if (strcmp(word, "void") == 0)
    return TOKEN_VOID;
  if (strcmp(word, "char") == 0)
    return TOKEN_CHAR;
  if (strcmp(word, "bool") == 0)
    return TOKEN_BOOL;
  if (strcmp(word, "str") == 0)
    return TOKEN_STR;
  if (strcmp(word, "ptr") == 0)
    return TOKEN_PTR;
  if (strcmp(word, "ary") == 0)
    return TOKEN_ARY;
  if (strcmp(word, "struct") == 0)
    return TOKEN_STRUCT;
  if (strcmp(word, "union") == 0)
    return TOKEN_UNION;

  // Integer Types
  if (strcmp(word, "i8") == 0)
    return TOKEN_I8;
  if (strcmp(word, "i16") == 0)
    return TOKEN_I16;
  if (strcmp(word, "i32") == 0)
    return TOKEN_I32;
  if (strcmp(word, "i64") == 0)
    return TOKEN_I64;
  if (strcmp(word, "i128") == 0)
    return TOKEN_I128;
  if (strcmp(word, "u8") == 0)
    return TOKEN_U8;
  if (strcmp(word, "u16") == 0)
    return TOKEN_U16;
  if (strcmp(word, "u32") == 0)
    return TOKEN_U32;
  if (strcmp(word, "u64") == 0)
    return TOKEN_U64;
  if (strcmp(word, "u128") == 0)
    return TOKEN_U128;

  // Float Types
  if (strcmp(word, "f16") == 0)
    return TOKEN_F16;
  if (strcmp(word, "f32") == 0)
    return TOKEN_F32;
  if (strcmp(word, "f64") == 0)
    return TOKEN_F64;
  if (strcmp(word, "f128") == 0)
    return TOKEN_F128;

  return IDENTIFIER;
}

TokenType get_process_type(const char *word) {
  // Compiler Keywords
  if (strcmp(word, "import") == 0)
    return TOKEN_IMPORT;
  if (strcmp(word, "define") == 0)
    return TOKEN_DEFINE;
  if (strcmp(word, "comptime") == 0)
    return TOKEN_COMPTIME;
  if (strcmp(word, "test") == 0)
    return TOKEN_TEST;

  return IDENTIFIER;
}
const char *Ttype_to_string(TokenType type) {
  switch (type) {
  case UNKNOWN:
    return "UNKNOWN";
  case END_TOKEN:
    return "EOF";

  // Value / Identifiers
  case IDENTIFIER:
    return "IDENTIFIER";
  case LITERAL_INT:
    return "LITERAL_INT";
  case LITERAL_FLOAT:
    return "LITERAL_FLOAT";
  case LITERAL_STRING:
    return "LITERAL_STRING";
  case LITERAL_CHAR:
    return "LITERAL_CHAR";

  // Literals
  case LITERAL_TRUE:
    return "TRUE";
  case LITERAL_FALSE:
    return "FALSE";

  // Function & Visibility
  case TOKEN_PUB:
    return "TOKEN_PUB";
  case TOKEN_FUNC:
    return "TOKEN_FUNC";
  case TOKEN_RETURN:
    return "TOKEN_RETURN";

  // Error Handling
  case TOKEN_ERROR:
    return "TOKEN_ERROR";
  case TOKEN_TRY:
    return "TOKEN_TRY";
  case TOKEN_CATCH:
    return "TOKEN_CATCH";

  // Control Flow
  case TOKEN_IF:
    return "TOKEN_IF";
  case TOKEN_ELSE:
    return "TOKEN_ELSE";
  case TOKEN_WHILE:
    return "TOKEN_WHILE";
  case TOKEN_FOR:
    return "TOKEN_FOR";
  case TOKEN_LOOP:
    return "TOKEN_LOOP";
  case TOKEN_BREAK:
    return "TOKEN_BREAK";
  case TOKEN_CONTINUE:
    return "TOKEN_CONTINUE";
  case TOKEN_GUARD:
    return "TOKEN_GUARD";

  // Determiners
  case TOKEN_VAR:
    return "TOKEN_VAR";
  case TOKEN_CONST:
    return "TOKEN_CONST";
  case TOKEN_VOLATILE:
    return "TOKEN_VOLATILE";

  // Base Types
  case TOKEN_VOID:
    return "TOKEN_VOID";
  case TOKEN_CHAR:
    return "TOKEN_CHAR";
  case TOKEN_STR:
    return "TOKEN_STR";
  case TOKEN_BOOL:
    return "TOKEN_BOOL";
  case TOKEN_PTR:
    return "TOKEN_PTR";
  case TOKEN_ARY:
    return "TOKEN_ARY";
  case TOKEN_STRUCT:
    return "TOKEN_STRUCT";
  case TOKEN_UNION:
    return "TOKEN_UNION";

  // Integer Types
  case TOKEN_I8:
    return "TOKEN_I8";
  case TOKEN_I16:
    return "TOKEN_I16";
  case TOKEN_I32:
    return "TOKEN_I32";
  case TOKEN_I64:
    return "TOKEN_I64";
  case TOKEN_I128:
    return "TOKEN_I128";
  case TOKEN_U8:
    return "TOKEN_U8";
  case TOKEN_U16:
    return "TOKEN_U16";
  case TOKEN_U32:
    return "TOKEN_U32";
  case TOKEN_U64:
    return "TOKEN_U64";
  case TOKEN_U128:
    return "TOKEN_U128";

  // Float Types
  case TOKEN_F16:
    return "TOKEN_F16";
  case TOKEN_F32:
    return "TOKEN_F32";
  case TOKEN_F64:
    return "TOKEN_F64";
  case TOKEN_F128:
    return "TOKEN_F128";

  // Compiler
  case TOKEN_IMPORT:
    return "TOKEN_IMPORT";
  case TOKEN_DEFINE:
    return "TOKEN_DEFINE";
  case TOKEN_COMPTIME:
    return "TOKEN_COMPTIME";
  case TOKEN_TEST:
    return "TOKEN_TEST";

  // Logic
  case TOKEN_AND:
    return "TOKEN_AND";
  case TOKEN_OR:
    return "TOKEN_OR";
  case TOKEN_NOT:
    return "TOKEN_NOT";

  // Operators & Math
  case TOKEN_ADD:
    return "TOKEN_ADD";
  case TOKEN_SUB:
    return "TOKEN_SUB";
  case TOKEN_DEV:
    return "TOKEN_DEV";
  case TOKEN_REM:
    return "TOKEN_REM";
  case TOKEN_ASSIGN:
    return "TOKEN_ASSIGN";
  case TOKEN_TYPE_ASSIGN:
    return "TOKEN_TYPE_ASSIGN";
  case TOKEN_ASSIGN_TYPE:
    return "TOKEN_ASSIGN_TYPE";

  // Symbols
  case TOKEN_O_PREN:
    return "TOKEN_O_PREN";
  case TOKEN_C_PREN:
    return "TOKEN_C_PREN";
  case TOKEN_O_BRACE:
    return "TOKEN_O_BRACE";
  case TOKEN_C_BRACE:
    return "TOKEN_C_BRACE";
  case TOKEN_O_BRACKET:
    return "TOKEN_O_BRACKET";
  case TOKEN_C_BRACKET:
    return "TOKEN_C_BRACKET";
  case TOKEN_SEMICOLON:
    return "TOKEN_SEMICOLON";
  case TOKEN_DOT:
    return "TOKEN_DOT";
  case TOKEN_COMMA:
    return "TOKEN_COMMA";
  case TOKEN_QUESTION_MARK:
    return "TOKEN_QUESTION_MARK";

  default:
    return "UNKNOWN__ID";
  }
}

void free_tokens(Token *token) {
  while (token != NULL) {
    Token *next = token->next;
    if (token->value) {
      free(token->value);
    }
    free(token);
    token = next;
  }
}

void print_tokens(const Token *token) {
  while (token != NULL && token->type != END_TOKEN) {
    if (!token->value) {
      printf("%-16s\t%lu:%-2lu\n", Ttype_to_string(token->type), token->line,
             token->col);
    } else {
      printf("%-16s\t%lu:%-2lu\t%s\n", Ttype_to_string(token->type),
             token->line, token->col, token->value);
    }
    token = token->next;
  }
}

#define COLOR_RED "\033[1;31m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[1;37m"
#define COLOR_RESET "\033[0m"

void print_lexer_error(Lexer *error, const char *buffer, const char *file,
                       const int err) {
  Lexer *curr = error;
  for (int i = 0; i < err; i++) {
    if (curr == NULL)
      break;

    printf("%serror:%s %s%s%s at %lu:%lu\n", COLOR_RED, COLOR_RESET,
           COLOR_WHITE, curr->msg, COLOR_RESET, curr->line, curr->col);

    printf("%s-->%s %s at %lu:%lu\n", COLOR_CYAN, COLOR_RESET, file, curr->line,
           curr->col);

    const char *prev_line_start = NULL;
    const char *curr_line_start = buffer;
    const char *prev_line_end;
    uint64_t line_counter = 1;

    const char *scan = buffer;
    while (line_counter < curr->line && *scan != '\0') {
      if (*scan == '\n') {
        prev_line_start = curr_line_start;
        prev_line_end = scan;
        curr_line_start = scan + 1;
        line_counter++;
      }
      scan++;
    }

    const char *curr_line_end = curr_line_start;
    while (*curr_line_end != '\n' && *curr_line_end != '\0') {
      curr_line_end++;
    }

    if (curr->line > 1 && prev_line_start != NULL) {
      int prev_len = (int)(prev_line_end - prev_line_start);
      if (prev_len > 0 && prev_line_start[prev_len - 1] == '\r')
        prev_len--;

      printf("%s%4lu |%s %.*s\n", COLOR_CYAN, curr->line - 1, COLOR_RESET,
             prev_len, prev_line_start);
    }

    int curr_len = (int)(curr_line_end - curr_line_start);

    if (curr_len > 0 && curr_line_start[curr_len - 1] == '\r')
      curr_len--;

    printf("%s%4lu |%s %.*s\n", COLOR_CYAN, curr->line, COLOR_RESET, curr_len,
           curr_line_start);

    printf("     %s|%s ", COLOR_CYAN, COLOR_RESET);

    uint64_t padding = (curr->col > 0) ? (curr->col - 1) : 0;
    for (uint64_t j = 0; j < padding; j++) {
      printf(" ");
    }

    int underline_len = (int)curr->len;
    if (underline_len < 2)
      underline_len = 2;

    int max_possible = curr_len - (int)padding;
    if (max_possible < 2)
      max_possible = 2;

    if (underline_len > max_possible) {
      underline_len = max_possible;
    }

    printf("%s^", COLOR_RED);
    for (int j = 0; j < underline_len - 1; j++) {
      printf("~");
    }
    printf("%s\n\n", COLOR_RESET);

    curr = curr->next;
  }

  free_lexer_error(error, err);
}

void free_lexer_error(Lexer *error, const int err) {
  for (int i = 0; i < err; i++) {
    if (error == NULL)
      break;
    Lexer *next = error->next;
    free(error);
    error = next;
  }
}