#pragma once

#include <stdbool.h>

typedef struct {
    int id;
    bool free;
} virtual_reg_t;

enum VirtualRegId {
	VIRTUAL_REG_ID_SP
};

#define VIRTUAL_REG_SP ((virtual_reg_t){.id = VIRTUAL_REG_ID_SP})

extern virtual_reg_t recent_reg;

virtual_reg_t imachine_alloc_reg(void);
void imachine_free_reg(virtual_reg_t reg);
