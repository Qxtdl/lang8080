#include <stdio.h>

#include <intention.h>
#include <util.h>
#include <parser.h>

#define INTENTION(_type, ...) \
	intention_put((intention_t){ \
		.type = _type, \
		__VA_ARGS__ \
	}); \

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
		case INTENTION_ADD:
			printf("add r%d, r%d\n", intention.data.op2.dest, intention.data.op2.src);
			break;
		case INTENTION_SUB:
			printf("sub r%d, r%d\n", intention.data.op2.dest, intention.data.op2.src);
			break;
		case INTENTION_ADD_IMM:
			printf("add imm r%d, %d\n", intention.data.opimm.dest, intention.data.opimm.value);
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
	int i = 0;
	ast_node_t *node;
	while ((node = ast_walk(expr))) {
		switch (node->type) {
			case TYPE_NUMBER:
                INTENTION(INTENTION_LOAD_IMM,
                    .data.load_imm.reg = (i == 0 ? eval : imachine_alloc_reg()),
                    .data.load_imm.value = node->data.number.value
				)
				break;
			case TYPE_OPERATOR:
				virtual_reg_t op = eval;
				ast_node_t *op2 = ast_walk(expr);
				switch (op2->type) {
					case TYPE_NUMBER:
	                    INTENTION(INTENTION_LOAD_IMM,
	                    .data.load_imm.reg = imachine_alloc_reg(),
	                    .data.load_imm.value = op2->data.number.value
	                );
	                default: break;
				}
				switch (node->data.operator.type) {
					case TOKEN_PLUS:
						INTENTION(INTENTION_ADD,
							.data.op2.dest = op,
							.data.op2.src = recent_reg
						)
						break;
					case TOKEN_MINUS:
						INTENTION(INTENTION_SUB,
							.data.op2.dest = op,
							.data.op2.src = recent_reg
						)
						break;
					default: break;
				}
				imachine_free_reg(recent_reg);
				break;
			default: break;
		}
	}
}

/* TODO: Should be applied to blocks */
static void intentionalize_function(ast_node_t *function) {
    if (function->data.function.is_prototype) return;
    INTENTION(INTENTION_FUNCTION_START,
		.data.function_start_end.label = function->data.function.name		
    )
    ast_node_t *node;
    while ((node = ast_walk((ast_node_ptr_t *)&function->data.function.body->data.body.body))) {
        switch (node->type) {
            case TYPE_VARIABLE_DECLARATION:
            	intentionalize_expr(&node->data.variable_declaration.expr);
            	switch (node->data.variable_declaration.type) {
            		case TOKEN_BYTE:
            		case TOKEN_WORD:
            			INTENTION(INTENTION_ADD_IMM,
							.data.opimm.dest = VIRTUAL_REG_SP,
							.data.opimm.value = 2
            			)
            			break;
           			default: break;
            	}
            	imachine_free_reg(recent_reg);
                break;
            default: break;
        }
    }
    INTENTION(INTENTION_FUNCTION_END,
		.data.function_start_end.label = function->data.function.name		
    )
}

void intention_intentionalize(void) {
    INTENTION(INTENTION_FUNCTION_START,
		.data.function_start_end.label = "start"	
    )
	INTENTION(INTENTION_LOAD_IMM,
		.data.load_imm.reg = VIRTUAL_REG_SP,
		.data.load_imm.value = 0xffff
	);
    INTENTION(INTENTION_FUNCTION_END,
		.data.function_start_end.label = "start"		
    ) 
    ast_node_t *node;
    while ((node = ast_walk(&root->data.body.body))) {
        switch (node->type) {
            case TYPE_FUNCTION: intentionalize_function(node); break;
            default: break;
        }
    }
}
