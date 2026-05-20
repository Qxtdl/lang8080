#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct {
    const char *name;
    uint16_t address;
} label_t;

extern label_t *labels;
extern size_t labels_size;

void labels_process(void);

typedef struct {
    const char *name;
    int value;
} equ_t;

extern equ_t *equs;
extern size_t equs_size;

void push_equ(equ_t equ);
