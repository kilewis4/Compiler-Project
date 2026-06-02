#ifndef TOKEN_H
#define TOKEN_H

// Separator tokens: ;, (, )
typedef enum
{
    TOKEN_SEMI,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
} TokenTypeSeparator;

// Keyword tokens: exit, etc.
typedef enum
{
    TOKEN_EXIT,
} TokenTypeKeyword;

// Literal tokens: integers, floats, strings
typedef enum
{
    TOKEN_INT,
} TokenTypeLiteral;

// General token types
typedef enum
{
    TOKEN_SEPARATOR,
    TOKEN_KEYWORD,
    TOKEN_LITERAL,
    TOKEN_EOF,
} TokenType;

// Union to hold different token data
typedef struct
{
    TokenType type;
    int line;
    int column;
    
    union
    {
        TokenTypeSeparator separator;
        TokenTypeKeyword keyword;
        struct
        {
            TokenTypeLiteral literal_type;
            char *value;  // For storing integer strings
        } literal;
    } data;
} Token;

// Function declarations
Token *token_create_separator(TokenTypeSeparator sep, int line, int column);
Token *token_create_keyword(TokenTypeKeyword kw, int line, int column);
Token *token_create_literal_int(char *value, int line, int column);
void token_print(Token *token);
void token_free(Token *token);

#endif
