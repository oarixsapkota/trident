cmake_minimum_required(VERSION 4.2)
project(trident C)

set(CMAKE_C_STANDARD 99)

add_executable(
        trident
        src/trident.c
        src/file/file.c
        src/file/file.h
        src/lexer/lexer.c
        src/lexer/lexer.h
        src/lexer/token.c
        src/lexer/token.h
        src/parser/ast.c
        src/parser/ast.h
        src/parser/parser.c
        src/parser/parser.h
)