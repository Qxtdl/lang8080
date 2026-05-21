#include <string.h>
#include <ctype.h>

#include <stdio.h>

#include <lexer.h>
#include <global.h>
#include <util.h>
#include <token.h>

token_t *_tokens = NULL;
size_t tokens_size = 0;

static void push_token(token_t token) {
    printf("%s %d\n", token.name, token.type);
    _tokens = srealloc(_tokens, ++tokens_size * sizeof(token_t));
    _tokens[tokens_size - 1] = token;
}

static char *text;

void lexer_init(char *_text) {
    text = _text;
}

bool is_punctuation(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == '=';
}

bool lexer_lex(void) {
    //for (volatile int i = 0; i < 50000000; i++);

    static char *token = NULL;
    if (token == NULL)
        token = text;

    if (*token == '\0')
        return false;

    while (isspace(*token)) token++;

    if (isdigit(*token)) {
        char *end;
        push_token((token_t){
            .type = TOKEN_NUMBER,
            .data.number.value = strtoul(token, &end, 10)
        });

        token = end;

        return true;
    }

    if (!strncmp(token, "//", 2)) {
        while (*(token++) != '\n');
    }

    for (size_t i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
        if (!strncmp(token, keywords[i], strlen(keywords[i]))
            //&& !isalnum(*(token + strlen(keywords[i])))
        ) {
            push_token((token_t){
                .name = strdup(keywords[i]),
                .type = i
            });

            token += strlen(keywords[i]);

            return true;
        }
    }
    // must be a identifier
    {
        char *identifier_start = token;
        while (isalnum(*++token));
        push_token((token_t){
            .name = strndup(identifier_start, token - identifier_start),
            .type = TOKEN_IDENTIFIER
        });

        return true;
    }
}

static size_t i = 0;

token_t *lexer_token_pop(void) {
    if (i >= tokens_size) return NULL;
    return &_tokens[i++];
}

token_t *lexer_token_peek(void) {
    if (i >= tokens_size) return NULL;
    return &_tokens[i];
}

token_t *lexer_token_expect(token_t *_token, enum Token token) {
    if (_token->type != token) {
        app_error("Expected \"%s\", got \"%s\"\n", keywords[token], _token->name)
    }
    return _token;
}

token_t *lexer_multiple_token_expect(token_t *token, enum Token tokens[]) {
    for (int i = 0; tokens[i] != TOKEN_LAST; i++) {
        if (token->type == tokens[i]) {
            return token;
        }
    }

    app_error("Expected anything else but \"%s\"\n", token->name)
}
