#include <util.h>

#include <imachine.h>

virtual_reg_t *scoreboard = NULL;
size_t scoreboard_size = 0;

virtual_reg_t recent_reg = {0};

virtual_reg_t imachine_alloc_reg(void) {
    for (size_t i = 0; i < scoreboard_size; i++) {
        if (scoreboard[i].free) {
            scoreboard[i].free = false;
            return scoreboard[i];
        }
    }

    scoreboard = srealloc(scoreboard, ++scoreboard_size * sizeof(virtual_reg_t));
    scoreboard[scoreboard_size - 1].free = false;
    scoreboard[scoreboard_size - 1].id = scoreboard_size;
    return (recent_reg = scoreboard[scoreboard_size - 1]);
}

void imachine_free_reg(virtual_reg_t reg) {
    for (size_t i = 0; i < scoreboard_size; i++) {
        if (scoreboard[i].id == reg.id) {
            scoreboard[i].free = true;
        }
    }    
}
