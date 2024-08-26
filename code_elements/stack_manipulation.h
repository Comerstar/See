
#pragma once
#include "../stack_elements/stack.h"

void add_to_string(Stack, char);
void add_to_num(Stack, int);
void add_void(Stack);
void add_binop(Stack, Bopo);
void add_lambda(Stack);
void eval_top(Bindings, Stack);
