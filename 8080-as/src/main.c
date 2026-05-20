#include <util.h>
#include <lexer.h>
#include <label.h>
#include <codegen.h>

#include <stdio.h>

int main(int argc, char **argv) {
    while (lexer_lex(read_file(argv[1])));
    labels_process();
    lexer_new_pass();
    printf("-------------------\n");
    while (lexer_lex(read_file(argv[1])));
    printf("-------------------\n");
    codegen_process(argv[2]);
}
