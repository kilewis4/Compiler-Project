#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "token.h"

Parser *parser_create(Token **tokens, int token_count)
{
    Parser *parser = malloc(sizeof(Parser));
    if (!parser) return NULL;
    
    parser->tokens = tokens;
    parser->token_count = token_count;
    parser->current = 0;
    
    return parser;
}

void parser_free(Parser *parser)
{
    free(parser);
}

static Token *parser_current_token(Parser *parser)
{
    if (parser->current < parser->token_count)
        return parser->tokens[parser->current];
    return NULL;
}

static Token *parser_peek_token(Parser *parser, int offset)
{
    if (parser->current + offset < parser->token_count)
        return parser->tokens[parser->current + offset];
    return NULL;
}

static void parser_advance(Parser *parser)
{
    parser->current++;
}

static int parser_match(Parser *parser, TokenType type)
{
    Token *token = parser_current_token(parser);
    if (token && token->type == type)
    {
        parser_advance(parser);
        return 1;
    }
    return 0;
}

// Forward declarations
static ASTNode *parser_parse_statement(Parser *parser);
static ASTNode *parser_parse_expression(Parser *parser);

static ASTNode *parser_parse_program(Parser *parser)
{
    ASTNode *program = malloc(sizeof(ASTNode));
    program->type = AST_PROGRAM;
    program->data.program.statements = malloc(sizeof(ASTNode *) * 64);
    program->data.program.statement_count = 0;
    
    while (parser_current_token(parser) && parser_current_token(parser)->type != TOKEN_EOF)
    {
        ASTNode *stmt = parser_parse_statement(parser);
        if (stmt)
        {
            program->data.program.statements[program->data.program.statement_count++] = stmt;
        }
    }
    
    return program;
}

static ASTNode *parser_parse_statement(Parser *parser)
{
    Token *token = parser_current_token(parser);
    
    if (!token) return NULL;
    
    // Match "exit" keyword
    if (token->type == TOKEN_KEYWORD && token->data.keyword == TOKEN_EXIT)
    {
        parser_advance(parser);
        
        // Expect '('
        if (!parser_match(parser, TOKEN_SEPARATOR))
        {
            fprintf(stderr, "Error: Expected '(' after 'exit'\n");
            return NULL;
        }
        
        // Parse expression
        ASTNode *expr = parser_parse_expression(parser);
        
        // Expect ')'
        if (!parser_match(parser, TOKEN_SEPARATOR))
        {
            fprintf(stderr, "Error: Expected ')' after expression\n");
            return NULL;
        }
        
        // Expect ';'
        if (!parser_match(parser, TOKEN_SEPARATOR))
        {
            fprintf(stderr, "Error: Expected ';' after expression\n");
            return NULL;
        }
        
        ASTNode *stmt = malloc(sizeof(ASTNode));
        stmt->type = AST_STATEMENT_EXIT;
        stmt->data.exit_stmt.expr = expr;
        
        return stmt;
    }
    
    return NULL;
}

static ASTNode *parser_parse_expression(Parser *parser)
{
    Token *token = parser_current_token(parser);
    
    if (!token) return NULL;
    
    if (token->type == TOKEN_LITERAL && token->data.literal.literal_type == TOKEN_INT)
    {
        parser_advance(parser);
        
        ASTNode *expr = malloc(sizeof(ASTNode));
        expr->type = AST_EXPR_INT;
        expr->data.int_expr.value = atoi(token->data.literal.value);
        
        return expr;
    }
    
    return NULL;
}

ASTNode *parser_parse(Parser *parser)
{
    return parser_parse_program(parser);
}

void ast_print(ASTNode *node)
{
    if (!node) return;
    
    switch (node->type)
    {
        case AST_PROGRAM:
            printf("PROGRAM\n");
            for (int i = 0; i < node->data.program.statement_count; i++)
            {
                printf("  ");
                ast_print(node->data.program.statements[i]);
            }
            break;
        case AST_STATEMENT_EXIT:
            printf("EXIT(\n");
            printf("    ");
            ast_print(node->data.exit_stmt.expr);
            printf("  )\n");
            break;
        case AST_EXPR_INT:
            printf("INT(%d)\n", node->data.int_expr.value);
            break;
    }
}

void ast_free(ASTNode *node)
{
    if (!node) return;
    
    switch (node->type)
    {
        case AST_PROGRAM:
            for (int i = 0; i < node->data.program.statement_count; i++)
            {
                ast_free(node->data.program.statements[i]);
            }
            free(node->data.program.statements);
            break;
        case AST_STATEMENT_EXIT:
            ast_free(node->data.exit_stmt.expr);
            break;
        case AST_EXPR_INT:
            break;
    }
    free(node);
}
