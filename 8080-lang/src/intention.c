#include <stdio.h>

#include <intention.h>
#include <util.h>
#include <parser.h>

intention_t *intentions = NULL;
size_t intentions_size = 0;

static const char *intention_print(intention_t intention) {
    switch (intention.type) {
        case INTENTION_FUNCTION_START: 
            printf("function start \"%s\"\n", intention.data.function_start_end.label);
            break;
        case INTENTION_FUNCTION_END: 
            printf("function end \"%s\"\n", intention.data.function_start_end.label);
            break;
        case INTENTION_LOAD_IMM: 
            printf("load imm r%d, %d\n", intention.data.load_imm.reg.id, intention.data.load_imm.value);
            break;
        default: break;
    }
}

static void intention_put(intention_t intention) {
    intentions = srealloc(intentions, ++intentions_size * sizeof(intention_t));
    intentions[intentions_size - 1] = intention;
    intention_print(intention);
}

static void intentionalize_expr(ast_node_ptr_t *expr) {
	virtual_reg_t eval = imachine_alloc_reg();
	ast_node_t *node;
	while ((node = ast_walk(expr))) {
		switch (node->type) {
			case TYPE_NUMBER:
                intention_put((intention_t){
                    .type = INTENTION_LOAD_IMM,
                    .data.load_imm.reg = eval,
                    .data.load_imm.value = node->data.number.value
                });
                break;
		}
	}
}

static void intentionalize_function(ast_node_t *function) {
    if (function->data.function.is_prototype) return;
    intention_put((intention_t){
        .type = INTENTION_FUNCTION_START,
        .data.function_start_end.label = function->data.function.name
    });
    ast_node_t *node;
    while ((node = ast_walk((ast_node_ptr_t *)&function->data.function.body->data.body.body))) {
        switch (node->type) {
            case TYPE_VARIABLE_DECLARATION:
            	intentionalize_expr(&node->data.variable_declaration.expr);
            	imachine_free_reg(recent_reg);
                break;
            default: break;
        }
    }
    intention_put((intention_t){
        .type = INTENTION_FUNCTION_END,
        .data.function_start_end.label = function->data.function.name
    });
}

void intention_intentionalize(void) {
    intention_put((intention_t){
        .type = INTENTION_FUNCTION_START,
        .data.function_start_end.label = "start"
    });
	intention_put((intention_t){
		.type = INTENTION_LOAD_IMM,
		.data.load_imm.reg = VIRTUAL_REG_SP,
		.data.load_imm.value = 0xffff
	});
    intention_put((intention_t){
        .type = INTENTION_FUNCTION_END,
        .data.function_start_end.label = "start"
    });	  
    ast_node_t *node;
    while ((node = ast_walk(&root->data.body.body))) {
        switch (node->type) {
            case TYPE_FUNCTION: intentionalize_function(node); break;
            default: break;
        }
    }
}
