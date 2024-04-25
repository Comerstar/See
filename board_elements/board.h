
#pragma once
#include "position.h"
#include <pthread.h>

struct CellListStruct {
    char c;
    Cell pos;
    struct CellListStruct *next;
};
typedef struct CellListStruct *CellList;

struct BoardStruct {
    CellList cells;
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    pthread_mutex_t lock;
};
typedef struct BoardStruct *Board;

char get_char_index(Board, int, int);
char get_char_cell(Board, Cell);

void add_char_index(Board, int, int, char);
void add_char_cell(Board, Cell, char);
