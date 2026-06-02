#ifndef PARSER_H
#define PARSER_H

#include "token.h"

// Abstract Syntax Tree (AST) node types
typedef enum
{
    AST_PROGRAM,
    AST_STATEMENT_EXIT,
    AST_EXPR_INT,
} ASTNodeType;

// AST Node structure
typedef struct ASTNode
{
    ASTNodeType type;
    union
    {
        struct
        {
            struct ASTNode **statements;
            int statement_count;
        } program;
        struct
        {
            struct ASTNode *expr;
        } exit_stmt;
        struct
        {
            int value;
        } int_expr;
    } data;
} ASTNode;

// Parser structure
typedef struct
{
    Token **tokens;
    int token_count;
    int current;
} Parser;

// Function declarations
Parser *parser_create(Token **tokens, int token_count);
void parser_free(Parser *parser);
ASTNode *parser_parse(Parser *parser);
void ast_print(ASTNode *node);
void ast_free(ASTNode *node);

#endif
