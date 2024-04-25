
#pragma once
#include "pointer.h"
#include "board_elements/board.h"
#include "stack_elements/stack.h"
#include "board_elements/position.h"

typedef void (*EndCallback) (Position pos, float a, Pointer p, Board b, Bindings bgs, Stack stk);

struct CoreReturnStruct {
    EndCallback callback;
    Position pos;
    float a;
};
typedef struct CoreReturnStruct CoreReturn;

typedef CoreReturn (*CoreFunction) (Position, float, Board, Bindings, Stack);


struct LanguageBindingStruct {
    char c;
    CoreFunction f;
    struct LanguageBindingStruct *next;
};
typedef struct LanguageBindingStruct *LanguageBinding;

struct LanguageBindingPairStruct {
    char c;
    CoreFunction f;
};
typedef struct LanguageBindingPairStruct LanguagePair;

CoreFunction find_language_binding(LanguageBinding, char);
LanguageBinding create_binding(LanguagePair* pairs);

void ray_trace(Pointer, Board, LanguageBinding); // The heart of the see interpreter
