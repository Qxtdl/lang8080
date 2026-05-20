#include <stdio.h>
#include <string.h>

#include <label.h>
#include <util.h>
#include <lexer.h>

label_t *labels = NULL;
size_t labels_size = 0;

int code_size = 0;

static void push_label(label_t label) {
    labels = srealloc(labels, ++labels_size * sizeof(label_t));
    labels[labels_size - 1] = label;
}

void labels_process(void) {
    for (size_t i = 0; i < lexemes_size; i++) {
        if (lexemes[i].type == LT_OP) {
            code_size += lexemes[i].data.instruction.eb + 1;
            printf("%s taking %d bytes\n", lexemes[i].data.instruction.name, lexemes[i].data.instruction.eb);
        }
        else if (lexemes[i].type == LT_LABEL) {
            push_label((label_t){
                .name = lexemes[i].data.label.name,
                .address = code_size
            });
        }
        else if (lexemes[i].type == LT_DIRECTIVE) {
            if (!strcmp(lexemes[i].data.directive.name, "org")) {
                code_size = lexemes[i].data.directive.data.org.address;
            }
        }
    }
}

/* equ */

equ_t *equs = NULL;
size_t equs_size = 0;

void push_equ(equ_t equ) {
    equs = srealloc(equs, ++equs_size * sizeof(equ_t));
    equs[equs_size - 1] = equ;
}
