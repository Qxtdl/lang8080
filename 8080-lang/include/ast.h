#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <token.h>

struct ast_node;

typedef struct {
    struct ast_node **ptrs;
    size_t size, walked;
} ast_node_ptr_t;

typedef struct ast_node {
    enum AstType {
        TYPE_BODY,
        TYPE_FUNCTION,
        TYPE_FUNCTION_ARG,
        TYPE_VARIABLE_DECLARATION,
        TYPE_IDENTIFIER,
        TYPE_NUMBER,
        TYPE_OPERATOR
    } type;
    union {
        struct {
            ast_node_ptr_t body;
        } body;
        struct {
            const char *name;
            ast_node_ptr_t args;
            enum Token return_type;
            struct ast_node *body;
            bool is_prototype;
            uint32_t prototype_address;
        } function;
        struct {
            const char *name;
            enum Token type;
        } function_arg;
        struct {
            const char *name;
            enum Token type;
            ast_node_ptr_t expr;
        } variable_declaration;
        struct {
        	const char *name;
        } identifier;
        struct {
        	int value;
        } number;
        struct {
        	const char *name;
        } operator;
    } data;
} ast_node_t;

ast_node_t *ast_node_create(enum AstType type);
ast_node_t *ast_node_insert(ast_node_ptr_t *node, ast_node_t *child);
ast_node_t *ast_walk(ast_node_ptr_t *parent);
