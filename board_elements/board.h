
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
    pthread_mutex_t count_lock;
    pthread_mutex_t access_lock;
    int reading;
    int writing;
    int read_queued;
    int write_queued;
};
typedef struct BoardStruct *Board;

char get_char_index(Board, int, int);
char get_char_cell(Board, Cell);

void add_char_index(Board, int, int, unsigned char chr);
void add_char_cell(Board, Cell, unsigned char chr);
