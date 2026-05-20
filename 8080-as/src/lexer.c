#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <lexer.h>
#include <util.h>
#include <label.h>

#define TOKEN_GET(tokenset) \
    for (size_t i = 0; i < sizeof((tokenset))/sizeof((tokenset)[0]); i++) \
        if (!strncmp(token, (tokenset)[i].name, strlen((tokenset)[i].name)) && \
            !isalnum(*(token + strlen((tokenset)[i].name))) && \
            token != code && \
            !isalnum(*(token - 1)) \
        )

char *token = NULL;
lexeme_t *lexemes = NULL;
size_t lexemes_size = 0;
bool second_pass = false;

void lexer_new_pass(void) {
    token = NULL;
    free(lexemes);
    lexemes = NULL;
    lexemes_size = 0;
    second_pass = true;
}

static void lexeme_push(lexeme_t lexeme) {
    printf("Pushing %d\n", lexeme.type);
    lexemes = srealloc(lexemes, ++lexemes_size * sizeof(lexeme_t));
    lexemes[lexemes_size - 1] = lexeme;
}

bool lexer_lex(char *code) {
    if (!token) {
        token = code;
    }

    // skip comments
    if (*token == ';' || *token == '#') {
        while (*(token++) != '\n');
    }

    if (isdigit(*token)) {
        printf("%zu\n", strtoul(token, NULL, 10));

        lexeme_push((lexeme_t){
            .type = LT_NUMBER,
            .data.number.value = strtoul(token, NULL, 10)
        });

        while (isdigit(*(token++)));
    }

    // push a directive?
    TOKEN_GET(directives) {
        printf("%s\n", directives[i].name);

        lexeme_t lexeme;
        lexeme.type = LT_DIRECTIVE;
        lexeme.data.directive.name = directives[i].name;


        if (!second_pass && !strcmp(directives[i].name, "equ")) {
            char *tmp = strdup(token);
            strtok(tmp, " ");
            char *name = strtok(NULL, " "),
                 *value = strtok(NULL, " ");

            push_equ((equ_t){
                .name = name,
                .value = strtoul(value, NULL, 10)
            });
        }
        if (!strcmp(directives[i].name, "org")) {
            char *tmp = strdup(token);
            strtok(tmp, " ");
            char *value = strtok(NULL, " ");

            lexeme.data.directive.data.org.address = strtoul(value, NULL, 10);
        }

        lexeme_push(lexeme);

        while (*(token++) != '\n');

        return true;
    }

    // push a label?
    if (*token == '.') {
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
    TOKEN_GET(instructions) {
        printf("%s\n", instructions[i].name);
        lexeme_push((lexeme_t){
            .type = LT_OP,
            .data.instruction = instructions[i]
        });

        token += strlen(instructions[i].name);
        
        return true;
    }

    // push a register lexeme?
    TOKEN_GET(regs) {
        printf("reg %s\n", regs[i].name);
        lexeme_push((lexeme_t){
            .type = LT_REGISTER,
            .data.reg = regs[i]
        });

        token += strlen(regs[i].name);
        
        return true;
    }    

    // label literal
    if (token != code && *(token - 1) != '.')
    for (size_t i = 0; i < labels_size; i++) {
        if (!strncmp(token, labels[i].name, strlen(labels[i].name)) &&
            !isalnum(*(token + strlen(labels[i].name)))
        ) {
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

    // equ literal
    if (second_pass)
    for (size_t i = 0; i < equs_size; i++) {
        if (!strncmp(token, equs[i].name, strlen(equs[i].name)) &&
            !isalnum(*(token + strlen(equs[i].name)))
        ) {
            lexeme_push((lexeme_t){
                .type = LT_NUMBER,
                .data.number = {
                    .value = equs[i].value
                }
            });
            
            token += strlen(equs[i].name);

            return true;
        }
    }

    if (*(token++) == '\0')
        return false;

    return true;
}

lexeme_t lexeme_pop(void) {
    static size_t i = 0;
    if (i == lexemes_size) {
        return (lexeme_t){.type = LT_TERMINATOR};
    }
    return lexemes[i++];
}
