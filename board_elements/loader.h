
#pragma once
#include "board.h"
#include <stdio.h>
#include "../pointer.h"

struct PointerListCellStruct {
    Pointer p;
    struct PointerListCellStruct *next;
};
typedef struct PointerListCellStruct *PointerListCell;

struct PointerListStruct {
    PointerListCell l;
    int length;
};
typedef struct PointerListStruct *PointerList;

PointerList p_list_new(void);
void p_list_free(PointerList);
void p_list_add(PointerList, Pointer);

Board load_file(FILE *, PointerList, void (*output) (char *), StackElement (*input) (void));
