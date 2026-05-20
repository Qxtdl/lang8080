#include <stdio.h>
#include <string.h>

#include <codegen.h>
#include <global.h>
#include <lexer.h>

FILE *fptr;

int location = 0;

static void put_instruction(instruction_t *_inst) {
    instruction_t inst = *_inst;

    if (inst.op == O_INVALID) return;
    
    fseek(fptr, location, SEEK_SET);

    switch (inst.op_type) {
        case OT_STACK_RP:
        case OT_RP: inst.op |= (inst.operands[0].id << 4); break;
        case OT_DEST_REG: inst.op |= (inst.operands[0].id << 3); break;
        case OT_DEST_REG_SRC_REG: inst.op |= (inst.operands[0].id << 3) | (inst.operands[1].id); break;
        case OT_SRC_REG: inst.op |= (inst.operands[0].id); break;
        case OT_INLINE_IMM:
        case OT_NONE:
        default:
        break;
    }

    if (inst.op != O_DB)
        fwrite(&inst.op, 1, 1, fptr);

    printf("writing %s\n", inst.name);

    if (inst.eb == EB_BYTE)
        fwrite(&inst.byte_imm, 1, 1, fptr);
    else if (inst.eb == EB_WORD || inst.eb == EB_BRANCH)
        fwrite(&inst.word_imm, 1, 2, fptr);

    _inst->op = O_INVALID;

    location = ftell(fptr);
}

void codegen_process(const char *bin) {
    fptr = fopen(bin, "wb");
    if (!fptr) {
        app_error("Failed to open the file %s\n", bin)
    }

    instruction_t inst = (instruction_t){.op = O_INVALID};
    lexeme_t lexeme;
    while (1) {
        lexeme = lexeme_pop();

        switch (lexeme.type) {
            case LT_OP: 
                printf("popped op\n");
                put_instruction(&inst);
                memset(&inst, 0, sizeof(inst));
                inst = lexeme.data.instruction;
                break;
            case LT_REGISTER:
                inst.operands[inst.operands_size++] = lexeme.data.reg; break;
                if (inst.operands_size >= 0) {
                    app_warn("Operand overflow")
                }
                break;
            case LT_NUMBER:
                if (inst.eb == EB_BYTE)
                    inst.byte_imm = lexeme.data.number.value;
                else if (inst.eb == EB_WORD || inst.eb == EB_BRANCH)
                    inst.word_imm = lexeme.data.number.value;
                else if (inst.op_type == OT_INLINE_IMM)
                    inst.op |= ((lexeme.data.number.value << 3) & 7);
                break;
            case LT_TERMINATOR: goto done;
            case LT_DIRECTIVE:
                printf("popped directive\n");
                put_instruction(&inst);
                if (!strcmp(lexeme.data.directive.name, "end")) {
                    goto done;
                }
                if (!strcmp(lexeme.data.directive.name, "org")) {
                    location = lexeme.data.directive.data.org.address;
                }
                break;
            case LT_LABEL: 
            default: break;
        }
    }
done:

}
