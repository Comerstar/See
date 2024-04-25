
#include <stdlib.h>
#include "stack.h"
#include <stdio.h>

void stk_free(Stack stk) {
    //stk_print(stk);
    StackCell s = stk->stack;
    StackCell next;
    //printf("%d\n", stk->len);
    int i = 0;
    while(s) {
        //printf("%d\n", i);
        i++;
        next = s->next;
        //printf("Freeing element: %d\n", s->elem->type);
        e_free(s->elem);
        //printf("Freeing cell\n");
        free(s);
        s = next;
    }
    free(stk);
}

bool stk_pop(Stack stk, int i, StackElement *ret) {
    StackCell s = stk->stack, *ptr = &stk->stack;
    while(s && i) {
        i--;
        ptr = &s->next;
        s = s->next;
    }
    if (s) {
        *ret = s->elem;
        *ptr = s->next;
        stk->len--;
        free(s);
        return true;
    } else {
        return false;
    }
}

bool stk_get(Stack stk, int i, StackElement *ret) {
    StackCell s = stk->stack;
    while(s && i) {
        i--;
        s = s->next;
    }
    if (s) {
        *ret = e_ref(s->elem);
        return true;
    } else {
        return false;
    }
}

void stk_push(Stack stk, StackElement e) {
    StackCell s = stk->stack;
    stk->stack = malloc(sizeof(struct StackStruct));
    *(stk->stack) = (struct StackCellStruct) {e_ref(e), s};
    stk->len++;
    return;
}

bool stk_remove(Stack stk, int i) {
    StackElement e;
    if (stk_pop(stk, i, &e)) {
        e_free(e);
        return true;
    }
    return false;
}

void stk_print(Bindings bgs, Stack stk) {
    StackCell s = stk->stack;
    int i = 0;
    while(s) {
        printf("%d: ", i++);
        if (s->elem) {
            char* str = e_to_str(s->elem);
            printf("%s\n", str);
            free(str);
        } else {
            printf("NULL\n");
        }
        s = s->next;
        //if (i > 10) break;
    }
}
