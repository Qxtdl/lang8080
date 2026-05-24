#pragma once

#include "imachine.h"

typedef struct {
    enum IntentionType {
        INTENTION_FUNCTION_START, // declare a func
        INTENTION_FUNCTION_END, // end of a func
        INTENTION_LOAD_IMM, // load imm into virt register
    } type;
    union {
        struct {
            const char *label;
        } function_start_end;
        struct {
            virtual_reg_t reg;
            int value;
        } load_imm;
    } data;
} intention_t;

void intention_intentionalize(void);
