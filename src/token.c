#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

Token *token_create_separator(TokenTypeSeparator sep, int line, int column)
{
    Token *token = malloc(sizeof(Token));
    if (!token) return NULL;
    
    token->type = TOKEN_SEPARATOR;
    token->line = line;
    token->column = column;
    token->data.separator = sep;
    
    return token;
}

Token *token_create_keyword(TokenTypeKeyword kw, int line, int column)
{
    Token *token = malloc(sizeof(Token));
    if (!token) return NULL;
    
    token->type = TOKEN_KEYWORD;
    token->line = line;
    token->column = column;
    token->data.keyword = kw;
    
    return token;
}

Token *token_create_literal_int(char *value, int line, int column)
{
    Token *token = malloc(sizeof(Token));
    if (!token) return NULL;
    
    token->type = TOKEN_LITERAL;
    token->line = line;
    token->column = column;
    token->data.literal.literal_type = TOKEN_INT;
    token->data.literal.value = malloc(strlen(value) + 1);
    if (!token->data.literal.value)
    {
        free(token);
        return NULL;
    }
    strcpy(token->data.literal.value, value);
    
    return token;
}

void token_print(Token *token)
{
    if (!token) return;
    
    printf("[%d:%d] ", token->line, token->column);
    
    switch (token->type)
    {
        case TOKEN_SEPARATOR:
            switch (token->data.separator)
            {
                case TOKEN_SEMI: printf("SEMI"); break;
                case TOKEN_OPEN_PAREN: printf("OPEN_PAREN"); break;
                case TOKEN_CLOSE_PAREN: printf("CLOSE_PAREN"); break;
            }
            break;
        case TOKEN_KEYWORD:
            switch (token->data.keyword)
            {
                case TOKEN_EXIT: printf("EXIT"); break;
            }
            break;
        case TOKEN_LITERAL:
            printf("INT(%s)", token->data.literal.value);
            break;
        case TOKEN_EOF:
            printf("EOF");
            break;
    }
    printf("\n");
}

void token_free(Token *token)
{
    if (!token) return;
    
    if (token->type == TOKEN_LITERAL && token->data.literal.value)
    {
        free(token->data.literal.value);
    }
    free(token);
}
