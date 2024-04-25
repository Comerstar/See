
#include "bindings.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

bool bgs_find(Bindings bindings, StackString name, StackElement *ret) {
    Binding binding = bindings->bindings;
    //printf("Searching for %s in:\n", str_to_str(name));
    //bgs_print(bindings);
    while (binding) {
        if (str_equal(binding->name, name)) {
            *ret = e_ref(binding->value);
            //printf("Found %s\n", str_to_str(name));
            return true;
        }
        binding = binding->next;
    }
    return false;
}

void bg_free(Binding binding) {
    if (binding) {
        binding->ref_count--;
        if (!binding->ref_count) {
            bg_free(binding->next);
            e_free(binding->value);
            str_free(binding->name);
            free(binding);
        }
    }
}

Binding bg_ref(Binding b) {
    if (b) {
        b->ref_count++;
    }
    return b;
}

void bgs_free(Bindings b) {
    if (b) {
        b->ref_count--;
        if (!b->ref_count) {
            bg_free(b->bindings);
            free(b);
        }
    }
}

Bindings bgs_ref(Bindings b) {
    if (b) {
        b->ref_count++;
    }
    return b;
}

void bgs_replace(Bindings bgs, StackString name, StackElement value) {
    Binding bg = bgs->bindings;
    while(bg) {
        if (str_equal(name, bg->name)) {
            bg->value = e_ref(value);
            return;
        }
        bg = bg->next;
    }
    bgs_add(bgs, name, value);
}

void bgs_add(Bindings bindings, StackString name, StackElement value) {
    Binding binding = bindings->bindings;
    bindings->bindings = malloc(sizeof(struct BindingStruct));
    *(bindings->bindings) = (struct BindingStruct){1, str_ref(name), e_ref(value), binding};
    return;
}

bool bgs_remove(Bindings bindings, StackString name) {
    Binding binding = bindings->bindings, next, *ptr = &(bindings->bindings);
    while (binding) {
        next = binding->next;
        if (str_equal(binding->name, name)) {
            bindings->bindings = bg_ref(binding->next);
            bg_free(binding);
            return true;
        }
        ptr = &(binding->next);
        binding = next;
    }
    return false;
}

Bindings bgs_copy(Bindings bindings) {
    if (bindings) {
        Bindings ret = malloc(sizeof(struct BindingsStruct));
        *ret = (struct BindingsStruct){1, bg_ref(bindings->bindings)};
        return ret;
    }
    return 0;
}

Bindings bgs_new(void) {
    Bindings ret = malloc(sizeof(struct BindingsStruct));
    *ret = (struct BindingsStruct){1, 0};
    return ret;
}

void bgs_print(Bindings bgs) {
    Binding b = bgs->bindings;
    while(b) {
        if (b->name) {
            char* str = str_to_str(b->name);
            printf("%s: ", str);
            free(str);
        } else {
            printf("NULL: ");
        }
        if (b->value) {
            char* str = e_to_str(b->value);
            printf("%s\n", str);
            free(str);
        } else {
            printf("NULL\n");
        }
        b = b->next;
    }
}
