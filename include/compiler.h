#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// TODO: Reorganize into proper modules
// Current implementation has all functionality in one file
// Future improvements:
// 1. Split into lexer/parser/codegen modules
// 2. Add proper error handling
// 3. Implement symbol table
// 4. Add more language features

// Token types
enum TokenType {
    _EXIT = 1
};

// Token structure
struct TOKEN {
    enum TokenType type;
    int value;
};

// Function declarations
void execute(void);
void printsentence(char* sentence, int newline);
void generate_for_exit(int value);
void codegen(struct TOKEN* token);
void tokenise(char* code);
char readfile(void);

#endif /* COMPILER_H */
