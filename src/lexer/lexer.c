#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "token.h"
#include <string.h>

// Left untouched as requested
char *sub_string(const size_t start, const size_t len, const char *buffer) {
  char *result = malloc(len);
  for (int i = 0; i < len; ++i) {
    result[i] = buffer[start + i];
  }
  result[len] = '\0';
  return result;
}

char *get_word(const char *buffer, size_t *index, uint64_t *col) {
  const size_t start = *index;
  size_t len = 0;
  while (isalnum(buffer[start + len]) || buffer[start + len] == '_') {
    ++len;
  }
  *col += len;
  *index += len;
  return sub_string(start, len, buffer);
}

char *get_num(const char *buffer, size_t *index, uint64_t *col,
              TokenType *type) {
  const size_t start = *index;
  size_t len = 0;
  *type = LITERAL_INT;
  while (isdigit(buffer[start + len]) || buffer[start + len] == '.') {
    if (buffer[start + len] == '.') {
      if (*type == LITERAL_FLOAT) {
        break;
      }
      *type = LITERAL_FLOAT;
    }
    ++len;
  }
  *col += len;
  *index += len;
  return sub_string(start, len, buffer);
}

typedef enum { STRING_MODE, CHAR_MODE } LiteralMode;

char *get_literal(const char *buffer, size_t *index, uint64_t *col, int *err,
                  LiteralMode mode) {
  ++*index;
  const size_t start = *index;
  size_t len = 0;
  ++*col;
  const char closing = mode == STRING_MODE ? '\"' : '\'';

  while (buffer[start + len] != '\0') {
    if (buffer[start + len] == '\\') {
      len++;
      if (buffer[start + len] == '\0') {
        ++*err;
        break;
      }
      len++;
      continue;
    }
    if (buffer[start + len] == closing)
      break;
    if (buffer[start + len] == '\n') {
      ++*err;
      break;
    }
    len++;
  }

  char *res = sub_string(start, len, buffer);
  *col += len;
  *index += len;

  // Only consume the closing quote if we actually stopped on it
  // This prevents skipping the \n and breaking line counters
  if (buffer[*index] == closing) {
    ++*col;
    ++*index;
  }

  return res;
}

Token *lexer(const char *input, Lexer **lex_error_head, int *error) {
  uint64_t line = 1, col = 1;
  size_t i = 0;

  Token *temp_head = add_token(UNKNOWN, NULL, 0, 1);
  Token *current = temp_head;

  Lexer *temp_error_head = add_lexer_error(NULL, 0, 0, 0);
  Lexer *current_error = temp_error_head;

  while (input[i] != '\0') {
    const char c = input[i];

    if (isspace(c)) {
      if (c == '\n') {
        line++;
        col = 0;
      }
      col++;
      i++;
      continue;
    }

    if (isalpha(c) || c == '_') {
      const uint64_t t_col = col;
      char *word = get_word(input, &i, &col);
      const TokenType type = get_keyword_type(word);
      if (type != IDENTIFIER) {
        current->next = add_token(type, NULL, line, t_col);
        free(word);
      } else {
        current->next = add_token(type, word, line, t_col);
      }
      current = current->next;
      continue;
    }

    if (isdigit(c) || (c == '.' && isdigit(input[i + 1]))) {
      const uint64_t t_col = col;
      TokenType type;
      char *num = get_num(input, &i, &col, &type);
      current->next = add_token(type, num, line, t_col);
      current = current->next;
      continue;
    }

    if (c == '\"' || c == '\'') {
      const uint64_t t_col = col;
      int err = 0;
      LiteralMode mode = c == '\"' ? STRING_MODE : CHAR_MODE;
      char *lit = get_literal(input, &i, &col, &err, mode);
      if (err) {
        char *msg = mode == STRING_MODE ? "unterminated String found"
                                        : "unterminated Char found";
        current_error->next = add_lexer_error(msg, line, t_col, strlen(lit));
        current_error = current_error->next;
        ++*error;
      }
      TokenType type = mode == STRING_MODE ? LITERAL_STRING : LITERAL_CHAR;
      current->next = add_token(type, lit, line, t_col);
      current = current->next;
      continue;
    }

    const uint64_t t_col = col;
    switch (c) {
    case '(':
      current->next = add_token(TOKEN_O_PREN, NULL, line, t_col);
      break;
    case ')':
      current->next = add_token(TOKEN_C_PREN, NULL, line, t_col);
      break;
    case '{':
      current->next = add_token(TOKEN_O_BRACE, NULL, line, t_col);
      break;
    case '}':
      current->next = add_token(TOKEN_C_BRACE, NULL, line, t_col);
      break;
    case '[':
      current->next = add_token(TOKEN_O_BRACKET, NULL, line, t_col);
      break;
    case ']':
      current->next = add_token(TOKEN_C_BRACKET, NULL, line, t_col);
      break;
    case ';':
      current->next = add_token(TOKEN_SEMICOLON, NULL, line, t_col);
      break;
    case ',':
      current->next = add_token(TOKEN_COMMA, NULL, line, t_col);
      break;
    case '.':
      current->next = add_token(TOKEN_DOT, NULL, line, t_col);
      break;
    case '?':
      current->next = add_token(TOKEN_QUESTION_MARK, NULL, line, t_col);
      break;
      case '+':
        if (input[i + 1] == '=') {
          current->next = add_token(TOKEN_ADD_ASSIGN, NULL, line, t_col);
          i++;
          col++;
        } else {
          current->next = add_token(TOKEN_ADD, NULL, line, t_col);
        }
      break;
      case '-':
        if (input[i + 1] == '=') {
          current->next = add_token(TOKEN_SUB_ASSIGN, NULL, line, t_col);
          i++;
          col++;
        } else {
          current->next = add_token(TOKEN_SUB, NULL, line, t_col);
        }
      break;
      case '%':
        if (input[i + 1] == '=') {
          current->next = add_token(TOKEN_REM_ASSIGN, NULL, line, t_col);
          i++;
          col++;
        } else {
          current->next = add_token(TOKEN_REM, NULL, line, t_col);
        }
      break;
      case '=':
        if (input[i + 1] == '=') {
          current->next = add_token(TOKEN_EQUAL, NULL, line, t_col);
          i++;
          col++;
        } else {
          current->next = add_token(TOKEN_ASSIGN, NULL, line, t_col);
        }
      break;

    case '&':
      if (input[i + 1] == '&') {
        current->next = add_token(TOKEN_AND, NULL, line, t_col);
        i++;
        col++;
      } else {
        current->next = add_token(UNKNOWN, NULL, line, t_col);
      }
      break;
    case '|':
      if (input[i + 1] == '|') {
        current->next = add_token(TOKEN_OR, NULL, line, t_col);
        i++;
        col++;
      } else {
        current->next = add_token(UNKNOWN, NULL, line, t_col);
      }
      break;
      case '!':
        if (input[i + 1] == '=') {
          current->next = add_token(TOKEN_NOT_EQUAL, NULL, line, t_col);
          i++;
          col++;
        } else {
          current->next = add_token(TOKEN_NOT, NULL, line, t_col);
        }
      break;

    case ':':
      if (input[i + 1] == '=') {
        current->next = add_token(TOKEN_TYPE_ASSIGN, NULL, line, t_col);
        i++;
        col++;
      } else {
        current->next = add_token(TOKEN_ASSIGN_TYPE, NULL, line, t_col);
      }
      break;

    case '/':
      if (input[i + 1] == '/') {
        while (input[i] != '\n' && input[i] != '\0') {
          i++;
          col++;
        }
        continue;
      }
      current->next = add_token(TOKEN_DEV, NULL, line, t_col);
      break;

    case '@':
      if (isalpha(input[i + 1])) {
        i++;
        col++;
        char *proc = get_word(input, &i, &col);
        const TokenType type = get_process_type(proc);
        if (type != IDENTIFIER) {
          current->next = add_token(type, NULL, line, t_col);
        } else {
          current->next = add_token(UNKNOWN, NULL, line, t_col);
          current_error->next =
              add_lexer_error("invalid hint found.", line, t_col, strlen(proc));
          current_error = current_error->next;
          ++*error;
        }
        free(proc);
        current = current->next;
        continue;
      }
      current->next = add_token(UNKNOWN, NULL, line, t_col);
      break;

    default:
      current->next = add_token(UNKNOWN, NULL, line, t_col);
      current_error->next =
          add_lexer_error("invalid character found", line, t_col, 1);
      current_error = current_error->next;
      ++*error;
      break;
    }

    i++;
    col++;
    current = current->next;
  }

  current->next = add_token(END_TOKEN, NULL, line, col);
  current_error->next = NULL;

  *lex_error_head = temp_error_head->next;
  free(temp_error_head);

  Token *actual_head = temp_head->next;
  free(temp_head);
  return actual_head;
}