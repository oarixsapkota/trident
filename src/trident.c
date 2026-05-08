#include <stdio.h>
#include <stdlib.h>

#include "file/file.h"
#include "lexer/lexer.h"
#include "lexer/token.h"

int main(void) {
  const char *file = "../test/test.tri";
  char *buffer = read_file(file);
  if (!buffer) {
    printf("Error reading test.tri\n");
    return 1;
  }

  Lexer *lex_error_head = NULL;
  int error_count = 0;
  Token *tokens = lexer(buffer, &lex_error_head, &error_count);
  if (error_count > 0) {
    fprintf(stderr, "Error count: %d\n", error_count);
    print_lexer_error(lex_error_head, buffer, file, error_count);
    free(buffer);
    free_tokens(tokens);
    return error_count;
  }
  free(buffer);

  print_tokens(tokens);
  free_tokens(tokens);

  return 0;
}
