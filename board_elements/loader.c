
#include "loader.h"
#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 100

PointerList p_list_new(void) {
    PointerList ret = malloc(sizeof(struct PointerListStruct));
    *ret = (struct PointerListStruct){0, 0};
    return ret;
}

void p_list_free(PointerList l) {
    PointerListCell c = l->l, n;
    while(c) {
        n = c->next;
        free(c);
        c = n;
    }
    free(l);
}

void p_list_add(PointerList l, Pointer p) {
    PointerListCell c = malloc(sizeof(struct PointerListCellStruct));
    *c = (struct PointerListCellStruct){p, l->l};
    l->l = c;
    l->length++;
}

Board load_file(FILE *fp, PointerList pl, void (*output) (char *), StackElement (*input) (void)) {
    char buffer[BUFFER_SIZE];
    int x = 0, y = 0, read;
    Board ret = malloc(sizeof(struct BoardStruct));
    *ret = (struct BoardStruct){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    if (pthread_mutex_init(&ret->access_lock, NULL) != 0 && pthread_mutex_init(&ret->count_lock, NULL) != 0) {
        printf("Failed to initialise board mutex\n");
        return ret;
    }
    while(read = fread(&buffer, sizeof(char), BUFFER_SIZE, fp)) {
        int i;
        for (i = 0; i < read; i++) {
            switch (buffer[i]) {
                case '\n':
                    x = 0;
                    y++;
                    break;
                case '.':
                    p_list_add(pl, p_new((Cell){x, y}, 0, pl->length, output, input));
                    x++;
                    break;
                default:
                    add_char_index(ret, x, y, buffer[i]);
                    x++;
                    break;
            }
        }
    }
    return ret;
}