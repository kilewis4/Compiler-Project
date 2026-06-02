#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    
    // Open the input file
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        fprintf(stderr, "Error: Could not open file '%s'\n", argv[1]);
        return 1;
    }
    
    // Phase 1: Lexical Analysis (Tokenization)
    printf("=== LEXING ===\n");
    Lexer *lexer = lexer_create(file);
    TokenStream *tokens = lexer_tokenize(lexer);
    token_stream_print(tokens);
    
    // Phase 2: Syntax Analysis (Parsing)
    printf("\n=== PARSING ===\n");
    Parser *parser = parser_create(tokens->tokens, tokens->count);
    ASTNode *ast = parser_parse(parser);
    printf("Abstract Syntax Tree:\n");
    ast_print(ast);
    
    // Cleanup
    ast_free(ast);
    parser_free(parser);
    token_stream_free(tokens);
    lexer_free(lexer);
    
    return 0;
}
