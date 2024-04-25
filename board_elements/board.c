
#include <stdlib.h>
#include "board.h"
#include "../utils/utils.h"

void add_cell(Board b, Cell c, char chr) {
    CellList l = b->cells;
    b->cells = malloc(sizeof(struct CellListStruct));
    *(b->cells) = (struct CellListStruct){chr, c, l};
}

char get_char_index(Board b, int x, int y) {
    return get_char_cell(b, (Cell){x, y});
}

char get_char_cell(Board b, Cell c) {
    pthread_mutex_lock(&b->lock);
    if (c.x < b->min_x || c.x > b->max_x || c.y < b->min_y || c.y > b->max_y) {
        pthread_mutex_unlock(&b->lock);
        return '\n';
    }
    CellList l = b->cells;
    while(l) {
        if (cell_equal(l->pos, c)) {
            pthread_mutex_unlock(&b->lock);
            return l->c;
        }
        l = l->next;
    }
    pthread_mutex_unlock(&b->lock);
    return ' ';
}

void add_char_index(Board b, int x, int y, char c) {
    add_char_cell(b, (Cell){x, y}, c);
}

void add_char_cell(Board b, Cell c, char chr) {
    pthread_mutex_lock(&b->lock);
    if (c.x < b->min_x || c.x > b->max_x || c.y < b->min_y || c.y > b->max_y) {
        add_cell(b, c, chr);
        b->min_x = i_min(c.x, b->min_x);
        b->max_x = i_max(c.x, b->max_x);
        b->min_y = i_min(c.y, b->min_y);
        b->max_y = i_max(c.y, b->max_y);
        pthread_mutex_unlock(&b->lock);
        return;
    }
    CellList l = b->cells;
    while(l) {
        if (cell_equal(l->pos, c)) {
            l->c = chr;
            pthread_mutex_unlock(&b->lock);
            return;
        }
        l = l->next;
    }
    add_cell(b, c, chr);
    pthread_mutex_unlock(&b->lock);
    return;
}
