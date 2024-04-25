
#include <stdlib.h>
#include <stdio.h>
#include "code_core.h"
#include "code_elements/cell_behaviours.h"

CoreFunction find_language_binding(LanguageBinding binding, char c) {
    if (binding) {
        if (binding->c == c) {
            return binding->f;
        } else {
            return find_language_binding(binding->next, c);
        }
    } else {
        return space_func;
    }
}

LanguageBinding create_binding(LanguagePair* pairs) {
    int i = 0;
    LanguageBinding ret = 0;
    while (pairs[i].c != '\0') {
        LanguageBinding next = ret;
        ret = malloc(sizeof(struct LanguageBindingStruct));
        *ret = (struct LanguageBindingStruct){pairs[i].c, pairs[i].f, next};
        i++;
    }
    return ret;
}

void ray_trace(Pointer pointer, Board board, LanguageBinding language) {
    if (pointer->running) {
        Position point = pos_from_cell(pointer->cell);
        float angle = pointer->angle;
        Stack stack = malloc(sizeof(struct StackStruct));
        *stack = (struct StackStruct){0, 0};
        //printf("Setup values\n");
        Bindings bindings = bgs_copy(pointer->bindings);

        StackString seen_line = 0;

        //printf("%d: Start core loop\n", pointer->id);
        while (1) {
            //printf("%f, %f, %f\n", full_position_x(point), full_position_y(point), angle);
            point = next_cell(point, angle);
            char c = get_char_cell(board, point.cell);

            seen_line = str_add(seen_line, c);
            //printf("%c, %f, %f, %f\n", c, full_position_x(point), full_position_y(point), angle);
            //printf("%d: Saw character: %c\n", pointer->id, c);
            CoreReturn next_state = find_language_binding(language, c) (point, angle, board, bindings, stack);

            //printf("\nPrinting stack\n");
            //stk_print(bindings, stack);
            //printf("Printed stack\n\n");

            //printf("\nPrinting bindings\n");
            //bgs_print(bindings);
            //printf("Printed bindings\n\n");

            if (next_state.callback) {
                //printf("End of line\n");
                next_state.callback(point, angle, pointer, board, bindings, stack);
                //printf("Printing bindings\n");
                //bgs_print(pointer->bindings);
                break;
            }
            point = next_state.pos;
            angle = next_state.a;
        }
        char* str_seen_line = str_to_str(seen_line);
        //printf("%d: Executed line: %s\n", pointer->id, str_seen_line);
        free(str_seen_line);
        str_free(seen_line);
        //printf("Freeing Bindings\n");
        bgs_free(bindings);
        //printf("Freeing Stack\n");
        stk_free(stack);
        //printf("Freed\n");
    }
}
