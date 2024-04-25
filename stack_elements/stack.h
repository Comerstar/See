
#pragma once
#include "stack_element.h"

struct StackCellStruct {
    StackElement elem;
    struct StackCellStruct *next;
};
typedef struct StackCellStruct *StackCell;

struct StackStruct {
    StackCell stack;
    int len;
};
typedef struct StackStruct *Stack;

void stk_free(Stack);
bool stk_pop(Stack, int, StackElement *);
bool stk_get(Stack, int, StackElement *);
void stk_push(Stack, StackElement);
bool stk_remove(Stack, int);
void stk_print(Bindings, Stack);
