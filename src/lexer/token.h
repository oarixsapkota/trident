#ifndef TRIDENT_TOKEN_H
#define TRIDENT_TOKEN_H
#include <stdint.h>

typedef enum {
    // helper
    UNKNOWN,
    END_TOKEN,
    // value
    IDENTIFIER,
    LITERAL_INT,
    LITERAL_FLOAT,
    LITERAL_STRING,
    LITERAL_CHAR,
    //literal
    LITERAL_TRUE,
    LITERAL_FALSE,
    // function
    TOKEN_PUB,
    TOKEN_FUNC,
    TOKEN_RETURN,
    // error
    TOKEN_ERROR,
    TOKEN_TRY,
    TOKEN_CATCH,
    // control flow
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_LOOP,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_GUARD,
    // determiners
    TOKEN_VAR,
    TOKEN_CONST,
    TOKEN_VOLATILE,
    // types
    TOKEN_VOID,
    TOKEN_CHAR,
    TOKEN_STR,
    TOKEN_BOOL,
    TOKEN_PTR,
    TOKEN_ARY,
    TOKEN_STRUCT,
    TOKEN_UNION,
    // int types
    TOKEN_I8,
    TOKEN_I16,
    TOKEN_I32,
    TOKEN_I64,
    TOKEN_I128,
    TOKEN_U8,
    TOKEN_U16,
    TOKEN_U32,
    TOKEN_U64,
    TOKEN_U128,
    // float types
    TOKEN_F16,
    TOKEN_F32,
    TOKEN_F64,
    TOKEN_F128,
    // compiler
    TOKEN_IMPORT,
    TOKEN_DEFINE,
    TOKEN_COMPTIME,
    // compiler seperator
    TOKEN_TEST,
    // logic
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    // operator
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_DEV,
    TOKEN_REM,
    // operator/ assign
    TOKEN_ADD_ASSIGN,
    TOKEN_SUB_ASSIGN,
    TOKEN_DEV_ASSIGN,
    TOKEN_REM_ASSIGN,
    // assign
    TOKEN_ASSIGN,
    TOKEN_TYPE_ASSIGN,
    TOKEN_ASSIGN_TYPE,
    // compare
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    // seperator
    TOKEN_O_PREN,
    TOKEN_C_PREN,
    TOKEN_O_BRACE,
    TOKEN_C_BRACE,
    TOKEN_O_BRACKET,
    TOKEN_C_BRACKET,
    TOKEN_SEMICOLON,
    // misc
    TOKEN_DOT,
    TOKEN_COMMA,
    TOKEN_QUESTION_MARK,
} TokenType;

typedef struct Token Token;
struct Token {
    TokenType type;
    uint64_t line;
    uint64_t col;
    char* value;
    Token* next;
};

typedef struct Lexer Lexer;
struct Lexer {
    char *msg;
    uint64_t line;
    uint64_t col;
    uint64_t len;
    Lexer *next;
};

Token *add_token(TokenType type, char *value,uint64_t line, uint64_t col);
Lexer *add_lexer_error(char *error, uint64_t line, uint64_t col, uint64_t len);
TokenType get_keyword_type(const char *word);
TokenType get_process_type(const char *word);
void print_tokens(const Token *token);
void free_tokens(Token *token);
void print_lexer_error(Lexer *error, const char *buffer, const char *file, int err);
void free_lexer_error(Lexer *error, int err);

#endif
