
#include "stack_manipulation.h"
#include <stdio.h>
#include "../bindings.h"

void add_to_string(Stack stk, char c) {
    StackElement e, elem;
    StackString str;
    if (stk_get(stk, 0, &e)) {
        switch (e->type) {
            case Int:
            case Float:
            case Binop:
            case Unop:
                str = str_from_chr(c);
                elem = e_from_str(str);
                stk_push(stk, elem);
                str_free(str);
                e_free(elem);
                break;
            case String:
                stk_remove(stk, 0);
                str = str_add(e->element.s, c);
                elem = e_from_str(str);
                stk_push(stk, elem);
                str_free(str);
                e_free(elem);
                break;
            case Void:
                stk_remove(stk, 0);
                str = str_from_chr(c);
                elem = e_from_str(str);
                stk_push(stk, elem);
                str_free(str);
                e_free(elem);
                break;
        }
        e_free(e);
    } else {
        StackString str = str_from_chr(c);
        StackElement elem = e_from_str(str);
        stk_push(stk, elem);
        str_free(str);
        e_free(elem);
    }
}

void add_to_num(Stack stk, int i) {
    StackElement e, elem;
    if (stk_get(stk, 0, &e)) {
        switch (e->type) {
            case Int:
                e->element.i *= 10;
                e->element.i += i;
                break;
            case Float:
                break;
            case String:
            case Binop:
            case Unop:
                elem = e_from_int(i);
                stk_push(stk, elem);
                e_free(elem);
                break;
            case Void:
                stk_remove(stk, 0);
                elem = e_from_int(i);
                stk_push(stk, elem);
                e_free(elem);
                break;
        }
        e_free(e);
    } else {
        StackElement elem = e_from_int(i);
        stk_push(stk, elem);
        e_free(elem);
    }
}

void add_void(Stack stk) {
    StackElement e, elem;
    if (stk_get(stk, 0, &e)) {
        switch (e->type) {
            case Void:
                break;
            default:
                StackElement elem = e_from_void();
                stk_push(stk, elem);
                e_free(elem);
                break;
        }
        e_free(e);
    } else {
        elem = e_from_void();
        stk_push(stk, elem);
        e_free(elem);
    }
}

void add_binop(Stack stk, Bopo op) {
    StackElement e1, e2, res;
    // printf("Adding binop to stack:\n");
    // stk_print(bgs_new(), stk);
    if (stk_get(stk, 1, &e1)) {
        if (stk_get(stk, 0, & e2)) {
            stk_remove(stk, 0);
            stk_remove(stk, 0);
            res = e_from_binop(op, e1, e2);
            stk_push(stk, res);
            // printf("Added binop to stack:\n");
            // stk_print(bgs_new(), stk);
            e_free(res);
            e_free(e2);
        }
        e_free(e1);
    }
}

void add_lambda(Stack stk) {
    StackElement e1, e2, res;
    // printf("Adding lambda to stack:\n");
    // stk_print(bgs_new(), stk);
    if (stk_get(stk, 1, &e1)) {
        // printf("%s\n", e_to_str(e1));
        switch (e1->type) {
            case String:
                if (stk_get(stk, 0, & e2)) {
                    stk_remove(stk, 0);
                    stk_remove(stk, 0);
                    res = e_from_lambda(e1->element.s, e2);
                    stk_push(stk, res);
                    // printf("Added lambda to stack:\n");
                    // stk_print(bgs_new(), stk);
                    e_free(res);
                    e_free(e2);
                }
                break;
        }
        e_free(e1);
    }
}

void eval_top(Bindings bgs, Stack stk) {
    StackElement e, evalled;
    //printf("Evalling top element of\n");
    //stk_print(bgs, stk);
    //printf("Getting top element\n");
    if (stk_get(stk, 0, &e)) {
        //printf("Removing top element\n");
        stk_remove(stk, 0);
        //printf("Evalling top element\n");
        evalled = e_eval(bgs, e);
        //printf("Pushing top element\n");
        stk_push(stk, evalled);
        e_free(evalled);
        e_free(e);
    }
}
