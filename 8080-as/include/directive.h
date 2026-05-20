#pragma once

typedef struct {
    const char *name;
    union {
        struct {
            int address;
        } org;
    } data;
} directive_t;

static const directive_t directives[] = {
    {"org"},
    {"equ"},
    {"end"},
};
