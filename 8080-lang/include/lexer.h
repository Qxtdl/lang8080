#include <stdbool.h>

#include <token.h>

typedef struct {
    const char *name;
    enum Token type;
    union {
        struct {
            int value;
        } number;
    } data;
} token_t;

void lexer_init(char *_text);
bool lexer_lex(void);
token_t *lexer_token_pop(void);
token_t *lexer_token_peek(void);
token_t *lexer_token_expect(token_t *_token, enum Token token);
token_t *lexer_multiple_token_expect(token_t *token, enum Token tokens[]);
