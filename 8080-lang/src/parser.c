#include <stdio.h>

#include <lexer.h>
#include <ast.h>

ast_node_t *root;

static ast_node_t *parse_expression(ast_node_ptr_t *expr) {
	token_t *token;
	while (1) {
		token = lexer_token_pop();
		ast_node_t *node;
		switch (token->type) {
			case TOKEN_IDENTIFIER: 
				printf("a identifier %s\n", token->name);
				node = ast_node_create(TYPE_IDENTIFIER);
				node->data.identifier.name = token->name;
				break;
			case TOKEN_NUMBER: 
				printf("a number %d\n", token->data.number.value);
				node = ast_node_create(TYPE_NUMBER);
				node->data.number.value = token->data.number.value;
				break;
			case TOKEN_PLUS:
			case TOKEN_MINUS:
			case TOKEN_EQUALS:
				printf("a operator %s\n", token->name);
				node = ast_node_create(TYPE_OPERATOR);
				node->data.operator.name = token->name;
				break;
			case TOKEN_SEMICOLON:
				goto done;			
			default: break;
		}
		ast_node_insert(expr, node);
	}
done:
}

static ast_node_t *parse_vardecl(void) {
    ast_node_t *var = ast_node_create(TYPE_VARIABLE_DECLARATION);
    var->data.variable_declaration.type = lexer_multiple_token_expect(lexer_token_pop(), (enum Token[]){TOKEN_BYTE, TOKEN_WORD, TOKEN_LAST})->type;
    var->data.variable_declaration.name = lexer_token_expect(lexer_token_pop(), TOKEN_IDENTIFIER)->name;
    printf("%s\n", var->data.variable_declaration.name);
    if (lexer_multiple_token_expect(lexer_token_pop(), (enum Token[]){TOKEN_EQUAL, TOKEN_SEMICOLON, TOKEN_LAST})->type != TOKEN_SEMICOLON)
    	parse_expression(&var->data.variable_declaration.expr);
    return var;
}

static ast_node_t *parse_body(void) {
    ast_node_t *body = ast_node_create(TYPE_BODY);
    lexer_token_expect(lexer_token_pop(), TOKEN_LBRACE);
    while (1) {
        // anything could be in a body, don't expect anything
        switch (lexer_token_peek()->type) {
            case TOKEN_BYTE:
            case TOKEN_WORD:
                ast_node_insert(&body->data.body.body, parse_vardecl());
                break;
            case TOKEN_RBRACE: lexer_token_pop(); goto done;
            default: break;
        }
    }
done:
    return body;
}

static ast_node_t *parse_function(void) {
    ast_node_t *function = ast_node_create(TYPE_FUNCTION);
    function->data.function.name = lexer_token_expect(lexer_token_pop(), TOKEN_IDENTIFIER)->name;
    lexer_token_expect(lexer_token_pop(), TOKEN_LPAREN);
    while (1) {
        token_t *token = lexer_multiple_token_expect(lexer_token_pop(), (enum Token[]){TOKEN_VOID, TOKEN_BYTE, TOKEN_WORD, TOKEN_LAST});
        if (token->type == TOKEN_VOID) {
            lexer_token_expect(lexer_token_pop(), TOKEN_RPAREN);
            break;
        }
        if (token->type == TOKEN_BYTE || token->type == TOKEN_WORD) {
            ast_node_t *arg = ast_node_create(TYPE_FUNCTION_ARG);
            arg->data.function_arg.type = token->type;
            arg->data.function_arg.name = lexer_token_expect(lexer_token_pop(), TOKEN_IDENTIFIER)->name;
            ast_node_insert(&function->data.function.args, arg);
            if (lexer_multiple_token_expect(lexer_token_pop(), (enum Token[]){TOKEN_COMMA, TOKEN_RPAREN, TOKEN_LAST})->type == TOKEN_RPAREN)
                break;
        }
    }
    function->data.function.return_type = lexer_multiple_token_expect(lexer_token_pop(), (enum Token[]){TOKEN_VOID, TOKEN_BYTE, TOKEN_WORD, TOKEN_LAST})->type;
    switch (lexer_multiple_token_expect(lexer_token_peek(), (enum Token[]){TOKEN_LBRACE, TOKEN_AT, TOKEN_LAST})->type) {
        case TOKEN_LBRACE: function->data.function.body = parse_body(); break;
        case TOKEN_AT:
            lexer_token_pop();
            function->data.function.is_prototype = true;
            function->data.function.prototype_address = lexer_token_expect(lexer_token_pop(), TOKEN_NUMBER)->data.number.value;
            lexer_token_expect(lexer_token_pop(), TOKEN_SEMICOLON);
            break;
        default: break;
    }
    return function;
}

void parser_parse(void) {
    root = ast_node_create(TYPE_BODY);

    token_t *token;
    while ((token = lexer_token_pop())) {
        printf("Popped %s %d\n", token->name, token->type);

        switch (token->type) {
            case TOKEN_FN:
                printf("Pushing a FN\n");
                ast_node_insert(&root->data.body.body, parse_function());
                break;
            default: break;
        }
    }
}
