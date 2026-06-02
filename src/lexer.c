#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "token.h"

#define INITIAL_TOKEN_CAPACITY 16

Lexer *lexer_create(FILE *file)
{
    Lexer *lexer = malloc(sizeof(Lexer));
    if (!lexer) return NULL;
    
    lexer->file = file;
    lexer->line = 1;
    lexer->column = 1;
    
    return lexer;
}

void lexer_free(Lexer *lexer)
{
    if (lexer)
    {
        if (lexer->file)
            fclose(lexer->file);
        free(lexer);
    }
}

// Helper: Add token to stream
static void token_stream_add(TokenStream *stream, Token *token)
{
    if (stream->count >= stream->capacity)
    {
        stream->capacity *= 2;
        stream->tokens = realloc(stream->tokens, sizeof(Token *) * stream->capacity);
    }
    stream->tokens[stream->count++] = token;
}

// Helper: Parse a number token
static Token *lexer_number(Lexer *lexer, char current)
{
    int start_column = lexer->column;
    char *value = malloc(64);
    int index = 0;
    
    while (isdigit(current) && current != EOF)
    {
        value[index++] = current;
        lexer->column++;
        current = fgetc(lexer->file);
    }
    
    // Put back the character that's not a digit
    if (current != EOF)
        ungetc(current, lexer->file);
    
    value[index] = '\0';
    
    Token *token = token_create_literal_int(value, lexer->line, start_column);
    free(value);
    
    return token;
}

// Helper: Parse a keyword token (or identifier)
static Token *lexer_keyword(Lexer *lexer, char current)
{
    int start_column = lexer->column;
    char *keyword = malloc(64);
    int index = 0;
    
    while (isalpha(current) && current != EOF)
    {
        keyword[index++] = current;
        lexer->column++;
        current = fgetc(lexer->file);
    }
    
    // Put back the character that's not alphabetic
    if (current != EOF)
        ungetc(current, lexer->file);
    
    keyword[index] = '\0';
    
    Token *token = NULL;
    
    // Check keyword match using strcmp correctly
    // strcmp returns 0 when strings match
    if (strcmp(keyword, "exit") == 0)
    {
        token = token_create_keyword(TOKEN_EXIT, lexer->line, start_column);
    }
    
    free(keyword);
    return token;
}

TokenStream *lexer_tokenize(Lexer *lexer)
{
    TokenStream *stream = malloc(sizeof(TokenStream));
    if (!stream) return NULL;
    
    stream->tokens = malloc(sizeof(Token *) * INITIAL_TOKEN_CAPACITY);
    stream->count = 0;
    stream->capacity = INITIAL_TOKEN_CAPACITY;
    
    char current = fgetc(lexer->file);
    
    while (current != EOF)
    {
        if (current == ' ' || current == '\t' || current == '\n' || current == '\r')
        {
            // Skip whitespace
            if (current == '\n')
            {
                lexer->line++;
                lexer->column = 1;
            }
            else
            {
                lexer->column++;
            }
        }
        else if (current == ';')
        {
            token_stream_add(stream, token_create_separator(TOKEN_SEMI, lexer->line, lexer->column));
            lexer->column++;
        }
        else if (current == '(')
        {
            token_stream_add(stream, token_create_separator(TOKEN_OPEN_PAREN, lexer->line, lexer->column));
            lexer->column++;
        }
        else if (current == ')')
        {
            token_stream_add(stream, token_create_separator(TOKEN_CLOSE_PAREN, lexer->line, lexer->column));
            lexer->column++;
        }
        else if (isdigit(current))
        {
            Token *token = lexer_number(lexer, current);
            if (token)
                token_stream_add(stream, token);
        }
        else if (isalpha(current))
        {
            Token *token = lexer_keyword(lexer, current);
            if (token)
                token_stream_add(stream, token);
        }
        else
        {
            // Unknown character - skip it
            fprintf(stderr, "Unknown character '%c' at %d:%d\n", current, lexer->line, lexer->column);
            lexer->column++;
        }
        
        current = fgetc(lexer->file);
    }
    
    // Add EOF token
    token_stream_add(stream, malloc(sizeof(Token)));
    stream->tokens[stream->count - 1]->type = TOKEN_EOF;
    stream->tokens[stream->count - 1]->line = lexer->line;
    stream->tokens[stream->count - 1]->column = lexer->column;
    
    return stream;
}

void token_stream_print(TokenStream *stream)
{
    if (!stream) return;
    
    printf("=== Token Stream ===\n");
    for (int i = 0; i < stream->count; i++)
    {
        token_print(stream->tokens[i]);
    }
    printf("====================\n");
}

void token_stream_free(TokenStream *stream)
{
    if (!stream) return;
    
    for (int i = 0; i < stream->count; i++)
    {
        token_free(stream->tokens[i]);
    }
    free(stream->tokens);
    free(stream);
}
