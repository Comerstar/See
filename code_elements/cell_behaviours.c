
#include "cell_behaviours.h"
#include "line_of_sight.h"
#include "stack_manipulation.h"
#include <stdio.h>

CoreReturn space_func(Position pos, float a, Board, Bindings, Stack) {
    return through(pos, a);
}

#define CHR_FUNC(name, chr) CoreReturn name##_func(Position pos, float a, Board, Bindings, Stack stk){\
    add_to_string(stk, chr);\
    return through(pos, a);\
}\

CHR_FUNC(a, 'a') CHR_FUNC(b, 'b') CHR_FUNC(c, 'c') CHR_FUNC(d, 'd') CHR_FUNC(e, 'e')
CHR_FUNC(f, 'f') CHR_FUNC(g, 'g') CHR_FUNC(h, 'h') CHR_FUNC(i, 'i') CHR_FUNC(j, 'j')
CHR_FUNC(k, 'k') CHR_FUNC(l, 'l') CHR_FUNC(m, 'm') CHR_FUNC(n, 'n') CHR_FUNC(o, 'o')
CHR_FUNC(p, 'p') CHR_FUNC(q, 'q') CHR_FUNC(r, 'r') CHR_FUNC(s, 's') CHR_FUNC(t, 't')
CHR_FUNC(u, 'u') CHR_FUNC(v, 'v') CHR_FUNC(w, 'w') CHR_FUNC(x, 'x') CHR_FUNC(y, 'y')
CHR_FUNC(z, 'z')

CoreReturn _0_func(Position pos, float a, Board, Bindings, Stack stk) {
    add_to_num(stk, 0);
    return through(pos, a);
}
CoreReturn _1_func(Position pos, float a, Board, Bindings, Stack stk) {
    add_to_num(stk, 1);
    return through(pos, a);
}
CoreReturn _2_func(Position pos, float a, Board, Bindings, Stack stk) {
    add_to_num(stk, 2);
    return through(pos, a);
}
CoreReturn _3_func(Position pos, float a, Board, Bindings, Stack stk) {
    add_to_num(stk, 3);
    return through(pos, a);
}
CoreReturn _4_func(Position pos, float a, Board, Bindings, Stack stk) {
    add_to_num(stk, 4);
    return through(pos, a);
}
CoreReturn _5_func(Position pos, float a, Board, Bindings, Stack stk) {
    add_to_num(stk, 5);
    return through(pos, a);
}
CoreReturn _6_func(Position pos, float a, Board, Bindings, Stack stk) {
    add_to_num(stk, 6);
    return through(pos, a);
}
CoreReturn _7_func(Position pos, float a, Board, Bindings, Stack stk) {
    add_to_num(stk, 7);
    return through(pos, a);
}
CoreReturn _8_func(Position pos, float a, Board, Bindings, Stack stk) {
    add_to_num(stk, 8);
    return through(pos, a);
}
CoreReturn _9_func(Position pos, float a, Board, Bindings, Stack stk) {
    add_to_num(stk, 9);
    return through(pos, a);
}

CoreReturn hmirror_func(Position pos, float a, Board, Bindings, Stack stk) {
    int count = 0;
    CoreReturn ret = ray_trace_through_cell(pos, a, (RayObject[]){
        {LineT, {.l = {-0.5f, 0, 0.5f, 0}}, ReflectT, {}, ReflectT, {}}, 
        {EndT}
    }, &count);
    if (count > 0) {
        add_void(stk);
    }
    return ret;
}

CoreReturn bmirror_func(Position pos, float a, Board, Bindings, Stack stk) {
    int count = 0;
    CoreReturn ret = ray_trace_through_cell(pos, a, (RayObject[]){
        {LineT, {.l = {-0.5f, -0.5f, 0.5f, 0.5f}}, ReflectT, {}, ReflectT, {}}, 
        {EndT}
    }, &count);
    if (count > 0) {
        add_void(stk);
    }
    return ret;
}

CoreReturn fmirror_func(Position pos, float a, Board, Bindings, Stack stk) {
    int count = 0;
    CoreReturn ret = ray_trace_through_cell(pos, a, (RayObject[]){
        {LineT, {.l = {-0.5f, 0.5f, 0.5f, -0.5f}}, ReflectT, {}, ReflectT, {}}, 
        {EndT}
    }, &count);
    if (count > 0) {
        add_void(stk);
    }
    return ret;
}

CoreReturn vmirror_func(Position pos, float a, Board, Bindings, Stack stk) {
    int count = 0;
    CoreReturn ret = ray_trace_through_cell(pos, a, (RayObject[]){
        {LineT, {.l = {0, -0.5f, 0, 0.5f}}, ReflectT, {}, ReflectT, {}}, 
        {EndT}
    }, &count);
    if (count > 0) {
        add_void(stk);
    }
    return ret;
}

CoreReturn xmirror_func(Position pos, float a, Board, Bindings, Stack stk) {
    int count = 0;
    CoreReturn ret = ray_trace_through_cell(pos, a, (RayObject[]){
        {LineT, {.l = {-0.5f, -0.5f, 0.5f, 0.5f}}, ReflectT, {}, ReflectT, {}}, 
        {LineT, {.l = {-0.5f, 0.5f, 0.5f, -0.5f}}, ReflectT, {}, ReflectT, {}}, 
        {EndT}
    }, &count);
    if (count > 0) {
        add_void(stk);
    }
    add_void(stk);
    return ret;
}

CoreReturn plusmirror_func(Position pos, float a, Board, Bindings, Stack stk) {
    int count = 0;
    CoreReturn ret = ray_trace_through_cell(pos, a, (RayObject[]){
        {LineT, {.l = {-0.5f, 0, 0.5f, 0}}, ReflectT, {}, ReflectT, {}}, 
        {LineT, {.l = {0, -0.5f, 0, 0.5f}}, ReflectT, {}, ReflectT, {}}, 
        {EndT}
    }, &count);
    if (count > 0) {
        add_void(stk);
    }
    add_void(stk);
    return ret;
}

CoreReturn omirror_func(Position pos, float a, Board, Bindings, Stack stk) {
    int count = 0;
    CoreReturn ret = ray_trace_through_cell(pos, a, (RayObject[]){
        {CircleT, {.c = {0, 0, 0.5f}}, ReflectT, {}, ReflectT, {}}, 
        {EndT}
    }, &count);
    if (count > 0) {
        add_void(stk);
    }
    add_void(stk);
    return ret;
}

CoreReturn condmirror_func(Position pos, float a, Board, Bindings bgs, Stack stk) {
    StackElement e;
    if (stk_get(stk, 0, &e)) {
        if (e_truthy(bgs, e)) {
            //printf("True reflect\n");
            return ray_trace_through_cell(pos, a, (RayObject[]){
                {LineT, {.l = {-0.5f, 0.5f, 0.5f, -0.5f}}, ReflectT, {}, ReflectT, {}}, 
                {EndT}
            }, 0);
        } else {
            //printf("False reflect\n");
            return ray_trace_through_cell(pos, a, (RayObject[]){
                {LineT, {.l = {-0.5f, -0.5f, 0.5f, 0.5f}}, ReflectT, {}, ReflectT, {}}, 
                {EndT}
            }, 0);
        }
        e_free(e);
    }
    return through(pos, a);
}

CoreReturn cap_a_func(Position pos, float a, Board, Bindings, Stack stk) {
    add_binop(stk, (Bopo){e_add, "ADD"});
    return through(pos, a);
}

CoreReturn cap_s_func(Position pos, float a, Board, Bindings, Stack stk) {
    add_binop(stk, (Bopo){e_sub, "SUB"});
    return through(pos, a);
}

CoreReturn cap_m_func(Position pos, float a, Board, Bindings, Stack stk) {
    add_binop(stk, (Bopo){e_mul, "MUL"});
    return through(pos, a);
}

CoreReturn cap_d_func(Position pos, float a, Board, Bindings, Stack stk) {
    add_binop(stk, (Bopo){e_div, "DIV"});
    return through(pos, a);
}

CoreReturn cap_p_func(Position pos, float a, Board, Bindings, Stack stk) {
    stk_remove(stk, 0);
    return through(pos, a);
}

CoreReturn percent_func(Position pos, float a, Board, Bindings, Stack stk) {
    add_binop(stk, (Bopo){e_mod, "MOD"});
    return through(pos, a);
}

CoreReturn dollar_func(Position pos, float a, Board, Bindings bgs, Stack stk) {
    eval_top(bgs, stk);
    return through(pos, a);
}
