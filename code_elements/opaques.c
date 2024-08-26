
#include "opaques.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

void default_end_callback(Position, float, Pointer p, Board, Bindings, Stack) {
    add_angle(p, 1);
}
void output_end_callback(Position, float, Pointer p, Board, Bindings bgs, Stack stk) {
    StackElement e, evalled;
    //printf("Printing stack value\n");
    if (stk_get(stk, 0, &e)) {
        evalled = e_eval(bgs, e);
        char *str = e_to_str(evalled);
        char *str2 = malloc(sizeof(char) * 100);
        sprintf(str2, "%s\n", str);

        p->output(str2);
        free(str);
        free(str2);
        e_free(evalled);
        e_free(e);
    } else {
        //printf("Empty stack when printing");
        p->output("");
    }
    add_angle(p, 1);
}
void input_end_callback(Position, float, Pointer p, Board, Bindings bgs, Stack stk) {
    StackElement e, input;
    //printf("Printing stack value\n");
    if (stk_get(stk, 0, &e) && e->type == String) {
        input = p->input();
        bgs_add(p->bindings, e->element.s, input);
        e_free(input);
        e_free(e);
    }
    add_angle(p, 1);
}
void halt_end_callback(Position, float, Pointer p, Board, Bindings, Stack) {
    p->running = false;
}
void left_end_callback(Position, float, Pointer p, Board, Bindings, Stack) {
    p->angle = M_PI;
}
void right_end_callback(Position, float, Pointer p, Board, Bindings, Stack) {
    p->angle = 0;
}
void up_end_callback(Position, float, Pointer p, Board, Bindings, Stack) {
    p->angle = 3 * M_PI_2;
}
void down_end_callback(Position, float, Pointer p, Board, Bindings, Stack) {
    p->angle = M_PI_2;
}
void jump_end_callback(Position pos, float, Pointer p, Board, Bindings, Stack) {
    p->cell = pos.cell;
}
void bind_end_callback(Position, float, Pointer p, Board, Bindings bgs, Stack stk) {
    StackElement name, value;
    //printf("Attempting to add binding to stack with elements:\n");
    //stk_print(bgs, stk);
    if (stk_get(stk, 1, &name)) {
        if (stk_get(stk, 0, &value)) {
            if (name->type == String) {
                //printf("Added binding\n");
                bgs_replace(p->bindings, name->element.s, value);
            }
            e_free(value);
        }
        e_free(name);
    }
    add_angle(p, 1);
}
void write_end_callback(Position, float, Pointer p, Board b, Bindings bgs, Stack stk) {
    StackElement value;
    // printf("Attempting to write to board\n");
    if (stk_get(stk, 0, &value)) {
        switch (value->type) {
            case String:
                if (value->element.s != NULL) {
                    add_char_cell(b, p->cell, value->element.s->c);
                }
                break;
            case Int:
                if (value->element.s != NULL) {
                    add_char_cell(b, p->cell, (unsigned char)(value->element.i % 256));
                }
                break;
        }
        e_free(value);
    }
    add_angle(p, 1);
}

CoreReturn default_end(Position p, float a, Board, Bindings, Stack) {
    return (CoreReturn){default_end_callback, p, a};
}
CoreReturn output_end(Position p, float a, Board, Bindings, Stack) {
    return (CoreReturn){output_end_callback, p, a};
}
CoreReturn input_end(Position p, float a, Board, Bindings, Stack) {
    return (CoreReturn){input_end_callback, p, a};
}
CoreReturn halt_end(Position p, float a, Board, Bindings, Stack) {
    return (CoreReturn){halt_end_callback, p, a};
}

CoreReturn left_end(Position p, float a, Board, Bindings, Stack) {
    return (CoreReturn){left_end_callback, p, a};
}
CoreReturn right_end(Position p, float a, Board, Bindings, Stack) {
    return (CoreReturn){right_end_callback, p, a};
}
CoreReturn up_end(Position p, float a, Board, Bindings, Stack) {
    return (CoreReturn){up_end_callback, p, a};
}
CoreReturn down_end(Position p, float a, Board, Bindings, Stack) {
    return (CoreReturn){down_end_callback, p, a};
}
CoreReturn jump_end(Position p, float a, Board, Bindings, Stack) {
    return (CoreReturn){jump_end_callback, p, a};
}
CoreReturn bind_end(Position p, float a, Board, Bindings, Stack) {
    return (CoreReturn){bind_end_callback, p, a};
}
CoreReturn write_end(Position p, float a, Board, Bindings, Stack) {
    return (CoreReturn){write_end_callback, p, a};
}
