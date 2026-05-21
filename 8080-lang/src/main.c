#include <stdio.h>

#include <util.h>
#include <lexer.h>
#include <parser.h>
#include <intention.h>

int main(int argc, char **argv) {
    printf("--------Stage 1: Lex--------\n");
    lexer_init(read_file(argv[1]));
    while (lexer_lex());
    printf("--------Stage 2: Parse--------\n");
    parser_parse();
    printf("--------Stage 3: Intentionalize--------\n");
    intention_intentionalize();
}
