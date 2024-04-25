
#pragma once
#include "stack_elements/stack_element.h"

bool bgs_find(Bindings, StackString, StackElement *);
void bg_free(Binding);
Binding bg_ref(Binding);
void bgs_free(Bindings);
Bindings bgs_ref(Bindings);
void bgs_replace(Bindings, StackString, StackElement);
void bgs_add(Bindings, StackString, StackElement);
bool bgs_remove(Bindings, StackString);
Bindings bgs_copy(Bindings);
Bindings bgs_new(void);
void bgs_print(Bindings);
