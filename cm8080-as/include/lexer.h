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
        LT_NUMBER
    } type;
    union {
        instruction_t instruction;
        _register_t _register;
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

bool lexer_lex(char *code);
void lexer_new_pass(void);
