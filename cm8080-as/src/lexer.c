#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <lexer.h>
#include <util.h>
#include <label.h>

#define TOKEN_GET(tokenset) \
    for (size_t i = 0; i < sizeof((tokenset))/sizeof((tokenset)[0]); i++) { \
        if (!strncmp(token, (tokenset)[i].name, strlen((tokenset)[i].name)) && \
            !isalnum(*(token + strlen((tokenset)[i].name)))) {

char *token = NULL;
lexeme_t *lexemes = NULL;
size_t lexemes_size = 0;
bool second_pass = false;

static void lexeme_push(lexeme_t lexeme) {
    lexemes = srealloc(lexemes, ++lexemes_size * sizeof(lexeme_t));
    lexemes[lexemes_size - 1] = lexeme;
}

void lexer_new_pass(void) {
    token = NULL;
    free(lexemes);
    lexemes = NULL;
    lexemes_size = 0;
    second_pass = true;
}

bool lexer_lex(char *code) {
    if (!token) {
        token = code;
    }

    // push a directive?
    if (*token == '#') {
        token++;
        TOKEN_GET(directives)
            printf("%s\n", directives[i].name);

            lexeme_push((lexeme_t){
                .type = LT_DIRECTIVE,
                .data.directive.name = directives[i].name
            });

            token += strlen(directives[i].name);

            return true;
        }}
    }

    // push a label?
    if (*token == '.' && !second_pass) {
        char *label_name = token+1,
             *tmp = label_name;

        int name_len = 0;
        for (; *tmp != ' ' && *tmp != '\t' && *tmp != '\n'; tmp++, name_len++);

        printf("*%s*\n", strndup(label_name, name_len));

        lexeme_push((lexeme_t){
            .type = LT_LABEL,
            .data.label.name = strndup(label_name, name_len)
        });

        token += name_len + 1;
    }

    // push a instruction lexeme?
    TOKEN_GET(instructions)
        printf("%s\n", instructions[i].name);
        lexeme_push((lexeme_t){
            .type = LT_OP,
            .data.instruction = instructions[i]
        });

        token += strlen(instructions[i].name);
        
        return true;
    }}

    // push a register lexeme?
    TOKEN_GET(_registers)
        printf("%s\n", _registers[i].name);
        lexeme_push((lexeme_t){
            .type = LT_REGISTER,
            .data._register = {
                .type = _registers[i].type,
                .name = _registers[i].name
            }
        });

        token += strlen(_registers[i].name);
        
        return true;
    }}

    // label literal
    if (second_pass && *(token-1) != '.')
    for (size_t i = 0; i < labels_size; i++) {
        if (!strncmp(token, labels[i].name, strlen(labels[i].name))) {
            printf("%s %d\n", labels[i].name, labels[i].address);
            lexeme_push((lexeme_t){
                .type = LT_NUMBER,
                .data.number = {
                    .value = labels[i].address
                }
            });
            
            token += strlen(labels[i].name);

            return true;
        }
    }

    if (*(token++) == '\0')
        return false;

    return true;
}
