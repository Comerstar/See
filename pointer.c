
#define _USE_MATH_DEFINES
#include "pointer.h"
#include "code_core.h"
#include <math.h>
#include <stdlib.h>

void add_angle(Pointer p, float a) {
    p->angle = fmod(p->angle + a, 2 * M_PI);
}

void set_angle(Pointer p, float a) {
    p->angle = fmod(a, 2 * M_PI);
}

void p_free(Pointer p) {
    bgs_free(p->bindings);
    free(p);
}

Pointer p_new(Cell c, float a, int id, void (*output) (char*), StackElement (*input) (void)) {
    Pointer ret = malloc(sizeof(struct PointerStruct));
    Bindings bindings = bgs_new();
    *ret = (struct PointerStruct){c, a, true, id, output, input, bindings};
    return ret;
}
