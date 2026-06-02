#ifndef LEXER_H
#define LEXER_H

#include "token.h"

// Dynamic array to hold tokens
typedef struct
{
    Token **tokens;
    int count;
    int capacity;
} TokenStream;

// Lexer structure
typedef struct
{
    FILE *file;
    int line;
    int column;
} Lexer;

// Function declarations
Lexer *lexer_create(FILE *file);
void lexer_free(Lexer *lexer);
TokenStream *lexer_tokenize(Lexer *lexer);
void token_stream_print(TokenStream *stream);
void token_stream_free(TokenStream *stream);

#endif
