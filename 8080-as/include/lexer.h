#pragma once

#include <stdbool.h>
#include <stddef.h>

#include <instruction.h>
#include <directive.h>

typedef struct {
    enum LexemeType {
        LT_OP,
        LT_REGISTER,
        LT_DIRECTIVE,
        LT_LABEL,
        LT_NUMBER,
        LT_TERMINATOR
    } type;
    union {
        instruction_t instruction;
        reg_t reg;
        directive_t directive;
        struct {
            const char *name;
        } label;
        struct {
            int value;
        } number;
    } data;
} lexeme_t;

extern lexeme_t *lexemes;
extern size_t lexemes_size;

void lexer_new_pass(void);
bool lexer_lex(char *code);
lexeme_t lexeme_pop(void);
