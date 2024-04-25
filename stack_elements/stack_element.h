
#pragma once
#include "stack_string.h"

struct StackElementStruct;
typedef struct StackElementStruct *StackElement;

struct BindingStruct {
    int ref_count;
    StackString name;
    StackElement value;
    struct BindingStruct *next;
};
typedef struct BindingStruct *Binding;

struct BindingsStruct {
    int ref_count;
    Binding bindings;
};
typedef struct BindingsStruct *Bindings;

enum StackTypeEnum {
    Int, Float, Void, Unit, String, Unop, Binop, Fun
};
typedef enum StackTypeEnum StackType;


struct StackFloatStruct {
    float value;
    int places;
};
typedef struct StackFloatStruct StackFloat;

typedef StackElement (*Uopf) (Bindings, StackElement);
struct UopStruct {
    Uopf op;
    char* name;
};
typedef struct UopStruct Uopo;

struct UnOperatorStruct {
    Uopo op;
    StackElement operand;
};
typedef struct UnOperatorStruct UnOperator;

typedef StackElement (*Bopf) (Bindings, StackElement, StackElement);
struct BopStruct {
    Bopf op;
    char* name;
};
typedef struct BopStruct Bopo;
struct BinOperatorStruct {
    Bopo op;
    StackElement left;
    StackElement right;
};
typedef struct BinOperatorStruct BinOperator;

struct LambdaStruct {
    StackString x;
    StackElement body;
};
typedef struct LambdaStruct Lambda;

struct StackElementStruct {
    StackType type;
    int ref_count;
    union {
        int i;
        StackFloat f;
        StackString s;
        UnOperator uop;
        BinOperator bop;
        Lambda l;
    } element;
};

void e_free(StackElement);
StackElement e_ref(StackElement);
StackElement e_from_int(int);
StackElement e_from_float(StackFloat);
StackElement e_from_str(StackString);
StackElement e_from_void(void);
StackElement e_from_unit(void);
StackElement e_from_unop(Uopo op, StackElement);
StackElement e_from_binop(Bopo op, StackElement, StackElement);
StackElement e_from_lambda(StackString, StackElement);
char* e_to_str(StackElement);
bool e_truthy(Bindings, StackElement);
StackElement e_eval(Bindings, StackElement);
StackElement e_add(Bindings, StackElement, StackElement);
StackElement e_sub(Bindings, StackElement, StackElement);
StackElement e_mul(Bindings, StackElement, StackElement);
StackElement e_div(Bindings, StackElement, StackElement);
StackElement e_mod(Bindings, StackElement, StackElement);
