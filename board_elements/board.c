#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "../utils/utils.h"

void read_lock(Board b) {
    pthread_mutex_lock(&b->count_lock);
    b->read_queued++;
    pthread_mutex_unlock(&b->count_lock);
    while (1) {
        pthread_mutex_lock(&b->count_lock);
        if (b->writing == 0 && b->write_queued == 0) {
            if (b->reading == 0) {
                pthread_mutex_lock(&b->access_lock);
                b->reading++;
                b->read_queued--;
                // printf("Read lock: Reading %d, Writing %d\n", b->reading, b->writing);
                pthread_mutex_unlock(&b->count_lock);
                return;
            }
            else {
                b->reading++;
                b->read_queued--;
                // printf("Read lock: Reading %d, Writing %d\n", b->reading, b->writing);
                pthread_mutex_unlock(&b->count_lock);
                return;
            }
        }
        pthread_mutex_unlock(&b->count_lock);
    }
}

void read_unlock(Board b) {
    pthread_mutex_lock(&b->count_lock);
    // printf("Read unlock: Reading %d, Writing %d\n", b->reading, b->writing);
    b->reading--;
    if (b->reading == 0) {
        pthread_mutex_unlock(&b->access_lock);
    }
    pthread_mutex_unlock(&b->count_lock);
}

void write_lock(Board b) {
    pthread_mutex_lock(&b->count_lock);
    b->write_queued++;
    pthread_mutex_unlock(&b->count_lock);
    while (1) {
        pthread_mutex_lock(&b->count_lock);
        if (b->reading == 0 && b->writing == 0) {
            pthread_mutex_lock(&b->access_lock);
            b->writing++;
            b->write_queued--;
            pthread_mutex_unlock(&b->count_lock);
            // printf("Write lock: Reading %d, Writing %d\n", b->reading, b->writing);
            return;
        }
        pthread_mutex_unlock(&b->count_lock);
    }
}

void write_unlock(Board b) {
    pthread_mutex_lock(&b->count_lock);
    // printf("Write unlock: Reading %d, Writing %d\n", b->reading, b->writing);
    b->writing--;
    if (b->writing == 0) {
        pthread_mutex_unlock(&b->access_lock);
    }
    pthread_mutex_unlock(&b->count_lock);
}

void add_cell(Board b, Cell c, char chr) {
    CellList l = b->cells;
    b->cells = malloc(sizeof(struct CellListStruct));
    *(b->cells) = (struct CellListStruct){chr, c, l};
}

char get_char_index(Board b, int x, int y) {
    return get_char_cell(b, (Cell){x, y});
}

char get_char_cell(Board b, Cell c) {
    read_lock(b);
    if (c.x < b->min_x || c.x > b->max_x || c.y < b->min_y || c.y > b->max_y) {
        read_unlock(b);
        return '\n';
    }
    CellList l = b->cells;
    while(l) {
        if (cell_equal(l->pos, c)) {
            read_unlock(b);
            return l->c;
        }
        l = l->next;
    }
    read_unlock(b);
    return ' ';
}

void add_char_index(Board b, int x, int y, unsigned char chr) {
    add_char_cell(b, (Cell){x, y}, chr);
}

void add_char_cell(Board b, Cell c, unsigned char chr) {
    write_lock(b);
    if (c.x < b->min_x || c.x > b->max_x || c.y < b->min_y || c.y > b->max_y) {
        add_cell(b, c, chr);
        b->min_x = i_min(c.x, b->min_x);
        b->max_x = i_max(c.x, b->max_x);
        b->min_y = i_min(c.y, b->min_y);
        b->max_y = i_max(c.y, b->max_y);
        write_unlock(b);
        return;
    }
    CellList l = b->cells;
    while(l) {
        if (cell_equal(l->pos, c)) {
            l->c = chr;
            write_unlock(b);
            return;
        }
        l = l->next;
    }
    add_cell(b, c, chr);
    write_unlock(b);
    return;
}
