#include <ast.h>
#include <util.h>

ast_node_t *ast_node_create(enum AstType type) {
    ast_node_t *node = scalloc(1, sizeof(ast_node_t));
    node->type = type;
    return node;
}

ast_node_t *ast_node_insert(ast_node_ptr_t *node, ast_node_t *child) {
    node->ptrs = srealloc(node->ptrs, ++node->size * sizeof(ast_node_t));
    node->ptrs[node->size - 1] = child;
    return child;
}

ast_node_t *ast_walk(ast_node_ptr_t *parent) {
    if (++parent->walked >= parent->size) 
        return NULL;
    return parent->ptrs[parent->walked - 1];
}
