
#include <stdlib.h>
#include <stdio.h>
#include "stack_element.h"
#include "../bindings.h"

Bindings bgs_new(void);
bool bgs_find(Bindings, StackString, StackElement *);

void e_free(StackElement e) {
    if (e) {
        e->ref_count--;
        if (!e->ref_count) {
            //printf("Freeing %p, %s\n", e, e_to_str(bgs_new(), e));
            switch (e->type) {
                case Int:
                    break;
                case Float:
                    break;
                case Void:
                    break;
                case Unit:
                    break;
                case String:
                    str_free(e->element.s);
                    break;
                case Unop:
                    UnOperator uop = e->element.uop;
                    StackElement operand = uop.operand;
                    if (operand) {
                        e_free(operand);
                    }
                    break;
                case Binop:
                    BinOperator bop = e->element.bop;
                    StackElement left = bop.left, right = bop.right;
                    if (left) {
                        e_free(left);
                    }
                    if (right) {
                        e_free(right);
                    }
                    break;
            }
        }
    }
}

StackElement e_ref(StackElement e) {
    if (e) {
        e->ref_count++;
    }
    return e;
}

StackElement e_from_int(int i) {
    StackElement ret = malloc(sizeof(struct StackElementStruct));
    *ret = (struct StackElementStruct) {Int, 1, {i}};
    return ret;
}

StackElement e_from_float(StackFloat f) {
    StackElement ret = malloc(sizeof(struct StackElementStruct));
    *ret = (struct StackElementStruct) {Float, 1, {.f = f}};
    return ret;
}

StackElement e_from_str(StackString str) {
    StackElement ret = malloc(sizeof(struct StackElementStruct));
    *ret = (struct StackElementStruct) {String, 1, {.s = str_ref(str)}};
    return ret;
}

StackElement e_from_void(void) {
    StackElement ret = malloc(sizeof(struct StackElementStruct));
    *ret = (struct StackElementStruct) {Void, 1};
    return ret;
}

StackElement e_from_unit(void) {
    StackElement ret = malloc(sizeof(struct StackElementStruct));
    *ret = (struct StackElementStruct) {Unit, 1};
    return ret;
}

StackElement e_from_unop(Uopo op, StackElement e) {
    StackElement ret = malloc(sizeof(struct StackElementStruct));
    *ret = (struct StackElementStruct) {Unop, 1, {.uop = {op, e_ref(e)}}};
    return ret;
}

StackElement e_from_binop(Bopo op, StackElement e1, StackElement e2) {
    StackElement ret = malloc(sizeof(struct StackElementStruct));
    *ret = (struct StackElementStruct) {Binop, 1, {.bop = {op, e_ref(e1), e_ref(e2)}}};
    return ret;
}

StackElement e_from_lambda(StackString x, StackElement b) {
    StackElement ret = malloc(sizeof(struct StackElementStruct));
    *ret = (struct StackElementStruct) {Fun, 1, {.l = {str_ref(x), e_ref(b)}}};
    return ret;
}

char* e_to_str(StackElement e) {
    char *str, *sres1, *sres2;
    //printf("Converting %p\n", e);
    //printf("Converting type %d\n", e->type);
    switch (e->type) {
        case Int:
            //printf("Int to string\n");
            str = malloc(sizeof(char) * 100);
            sprintf(str, "%d", e->element.i);
            return str;
        case Float:
            //printf("Float to string\n");
            str = malloc(sizeof(char) * 100);
            sprintf(str, "%f", e->element.f.value);
            return str;
        case String:
            //printf("String to string\n");
            return str_to_str(e->element.s);
        case Void:
            //printf("Void to string\n");
            str = malloc(sizeof(char) * 100);
            sprintf(str, "_");
            return str;
        case Unit:
            //printf("Unit to string\n");
            str = malloc(sizeof(char) * 100);
            sprintf(str, "()");
            return str;
        case Unop:
            str = malloc(sizeof(char) * 100);
            sres1 = e_to_str(e->element.uop.operand);
            sprintf(str, "%s(%s)", e->element.uop.op.name, sres1);
            free(sres1);
            return str;
        case Binop:
            str = malloc(sizeof(char) * 100);
            //printf("Op to string\n");
            sres1 = e_to_str(e->element.bop.left);
            sres2 = e_to_str(e->element.bop.right);
            sprintf(str, "%s(%s, %s)", e->element.bop.op.name, sres1, sres2);
            free(sres1);
            free(sres2);
            return str;
        case Fun:
            str = malloc(sizeof(char) * 100);
            sres1 = str_to_str(e->element.l.x);
            sres2 = e_to_str(e->element.l.body);
            sprintf(str, "%s(%s, %s)", e->element.bop.op.name, sres1, sres2);
            free(sres1);
            free(sres2);
            return str;
    }
}

bool e_truthy(Bindings bgs, StackElement e) {
    StackElement res;
    bool ret;
    switch (e->type) {
        case Int:
            return e->element.i != 0;
        case Float:
            return e->element.f.value != 0.0;
        case String:
            if (bgs_find(bgs, e->element.s, &res)) {
                ret = e_truthy(bgs, res);
                e_free(res);
                return ret;
            } else {
                return e->element.s;
            }
        case Void:
            return false;
        case Unit:
            return false;
        case Unop:
            res = e_eval(bgs, e);
            ret = e_truthy(bgs, res);
            e_free(res);
            return ret;
        case Binop:
            //printf("Truthy evaling\n");
            res = e_eval(bgs, e);
            //printf("Truthy truthying\n");
            ret = e_truthy(bgs, res);
            //printf("Truthy freeing\n");
            e_free(res);
            return ret;
    }
    return false;
}

// Always creates a new element which must be freed.
StackElement e_eval(Bindings bgs, StackElement e) {
    StackElement elem;
    switch (e->type) {
        case Int:
            return e_from_int(e->element.i);
        case Float:
            return e_from_float(e->element.f);
        case String:
            if (bgs_find(bgs, e->element.s, &elem)) {
                return elem;
            } else {
                return e_from_str(e->element.s);
            }
        case Void:
            return e_from_void();
        case Unit:
            return e_from_unit();
        case Unop:
            UnOperator uop = e->element.uop;
            return uop.op.op(bgs, uop.operand);
        case Binop:
            BinOperator bop = e->element.bop;
            return bop.op.op(bgs, bop.left, bop.right);
    }
    //printf("Invalid object evalled\n");
    return e_from_void();
}

StackElement e_evaling_binop(StackElement (* op) (Bindings, StackElement, StackElement), Bindings bgs, StackElement e1, StackElement e2) {
    StackElement res1, res2, ret;
    switch (e1->type) {
        case Int:
        case Float:
        case Void:
        case Unit:
        case Fun:
            switch (e2->type) {
                case Int:
                case Float:
                case Void:
                case Unit:
                case Fun:
                    return op(bgs, e1, e2);
                case String:
                    if (bgs_find(bgs, e2->element.s, &res2)) {
                        ret = op(bgs, e1, res2);
                        e_free(res2);
                        return ret;
                    } else {
                        return op(bgs, e1, e2);
                    }
                case Unop:
                case Binop:
                    res2 = e_eval(bgs, e2);
                    ret = op(bgs, e1, res2);
                    e_free(res2);
                    return ret;
            }
            break;
        case String:
            if (bgs_find(bgs, e1->element.s, &res1)) {
                switch (e2->type) {
                    case Int:
                    case Float:
                    case Void:
                    case Unit:
                    case Fun:
                        ret = op(bgs, res1, e2);
                        e_free(res1);
                        return ret;
                    case String:
                        if (bgs_find(bgs, e2->element.s, &res2)) {
                            ret = op(bgs, res1, res2);
                            e_free(res2);
                            e_free(res1);
                            return ret;
                        } else {
                            ret = op(bgs, res1, e2);
                            e_free(res1);
                            return ret;
                        }
                    case Unop:
                    case Binop:
                        res2 = e_eval(bgs, e2);
                        ret = op(bgs, res1, res2);
                        e_free(res2);
                        e_free(res1);
                        return ret;
                }
            } else {
                switch (e2->type) {
                    case Int:
                    case Float:
                    case Void:
                    case Unit:
                    case Fun:
                        return op(bgs, e1, e2);
                    case String:
                        if (bgs_find(bgs, e2->element.s, &res2)) {
                            ret = op(bgs, e1, res2);
                            e_free(res2);
                            return ret;
                        } else {
                            return op(bgs, e1, e2);
                        }
                    case Unop:
                    case Binop:
                        res2 = e_eval(bgs, e2);
                        ret = op(bgs, e1, res2);
                        e_free(res2);
                        return ret;
                }
            }
            break;
        case Unop:
        case Binop:
            switch (e2->type) {
                case Int:
                case Float:
                case Void:
                case Unit:
                case Fun:
                    res1 = e_eval(bgs, e1);
                    ret = op(bgs, res1, e2);
                    e_free(res1);
                    return ret;
                case String:
                    res1 = e_eval(bgs, e1);
                    if (bgs_find(bgs, e2->element.s, &res2)) {
                        ret = op(bgs, res1, res2);
                        e_free(res2);
                        e_free(res1);
                        return ret;
                    } else {
                        ret = op(bgs, res1, e2);
                        e_free(res1);
                        return ret;
                    }
                case Unop:
                case Binop:
                    res1 = e_eval(bgs, e1);
                    res2 = e_eval(bgs, e2);
                    ret = op(bgs, res1, res2);
                    e_free(res1);
                    e_free(res2);
                    return ret;
            }
            break;
    }

}

StackElement e_conc_add(Bindings bgs, StackElement e1, StackElement e2) {
    switch (e1->type) {
        case Int:
            switch (e2->type) {
                case Int:
                    return e_from_int(e1->element.i + e2 ->element.i);
            }
    }
    printf("Invalid object added\n");
    return e_from_void();
}

StackElement e_add(Bindings bgs, StackElement e1, StackElement e2) {
    return e_evaling_binop(e_conc_add, bgs, e1, e2);
}

StackElement e_conc_sub(Bindings bgs, StackElement e1, StackElement e2) {
    switch (e1->type) {
        case Int:
            switch (e2->type) {
                case Int:
                    return e_from_int(e1->element.i - e2 ->element.i);
            }
    }
    //printf("Invalid object subbed\n");
    return e_from_void();
}

StackElement e_sub(Bindings bgs, StackElement e1, StackElement e2) {
    return e_evaling_binop(e_conc_sub, bgs, e1, e2);
}

StackElement e_conc_mul(Bindings bgs, StackElement e1, StackElement e2) {
    switch (e1->type) {
        case Int:
            switch (e2->type) {
                case Int:
                    return e_from_int(e1->element.i * e2 ->element.i);
            }
    }
    return e_from_void();
}

StackElement e_mul(Bindings bgs, StackElement e1, StackElement e2) {
    return e_evaling_binop(e_conc_mul, bgs, e1, e2);
}

StackElement e_conc_div(Bindings bgs, StackElement e1, StackElement e2) {
    switch (e1->type) {
        case Int:
            switch (e2->type) {
                case Int:
                    return e_from_int(e1->element.i / e2 ->element.i);
            }
    }
    return e_from_void();
}

StackElement e_div(Bindings bgs, StackElement e1, StackElement e2) {
    return e_evaling_binop(e_conc_div, bgs, e1, e2);
}

StackElement e_conc_mod(Bindings bgs, StackElement e1, StackElement e2) {
    switch (e1->type) {
        case Int:
            switch (e2->type) {
                case Int:
                    return e_from_int(e1->element.i % e2 ->element.i);
            }
    }
    return e_from_void();
}

StackElement e_mod(Bindings bgs, StackElement e1, StackElement e2) {
    return e_evaling_binop(e_conc_mod, bgs, e1, e2);
}

StackElement e_conc_apply(Bindings bgs, StackElement e1, StackElement e2) {
    switch (e1->type) {
        case Fun:
            // printf("Applying\n");
            Bindings new_bgs = bgs_copy(bgs);
            bgs_add(new_bgs, e1->element.l.x, e_ref(e2));
            StackElement res = e_eval(new_bgs, e1->element.l.body);
            bgs_free(new_bgs);
            return res;
    }
    // printf("Applying to non-lambda %s\n", e_to_str(e1));
    bgs_print(bgs);
    return e_from_void();
}

StackElement e_apply(Bindings bgs, StackElement e1, StackElement e2) {
    // printf("Applying\n");
    return e_evaling_binop(e_conc_apply, bgs, e1, e2);
}
