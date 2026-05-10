#include <stdio.h>

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
        }
        else if (lexemes[i].type == LT_LABEL) {
            push_label((label_t){
                .name = lexemes[i].data.label.name,
                .address = code_size
            });
        }
    }
}
