
#pragma once
#include "board_elements/position.h"
#include "bindings.h"
#include <stdbool.h>
#include "board_elements/board.h"

struct PointerStruct {
    Cell cell;
    float angle;
    bool running;
    int id;
    void (*output) (char*);
    StackElement (*input) (void);
    Bindings bindings;
};
typedef struct PointerStruct *Pointer;

void p_free(Pointer);
Pointer p_new(Cell, float, int, void (*output) (char*), StackElement (*input) (void));
void add_angle(Pointer, float);
void set_angle(Pointer, float);
