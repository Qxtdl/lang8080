#include <stdio.h>

#include <parser.h>

static void intentionalize_function(ast_node_t *node) {
    
}

void intention_intentionalize(void) {
    ast_node_t *node;
    while ((node = ast_walk(&root->data.body.body))) {
        switch (node->type) {
            case TOKEN_FN: intentionalize_function(node); break;
            default: break;
        }
    }
}
