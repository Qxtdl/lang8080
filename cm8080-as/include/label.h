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
