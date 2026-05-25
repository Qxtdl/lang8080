#pragma once

#include "imachine.h"

typedef struct {
    enum IntentionType {
        INTENTION_FUNCTION_START, // declare a func
        INTENTION_FUNCTION_END, // end of a func
        INTENTION_LOAD_IMM, // load imm into virt register
        INTENTION_ADD, // add reg to reg
        INTENTION_SUB, // subtract reg from reg
        INTENTION_ADD_IMM, // add immediate to reg
    } type;
    union {
        struct {
            const char *label;
        } function_start_end;
        struct {
            virtual_reg_t reg;
            int value;
        } load_imm;
        struct {
        	virtual_reg_t dest, src;
        } op2;
        struct {
       		virtual_reg_t dest;
       		int value;
        } opimm;
    } data;
} intention_t;

void intention_intentionalize(void);
