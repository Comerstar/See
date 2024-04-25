
#pragma once
#include <stdbool.h>

struct StackStringStruct {
    char c;
    int ref_count;
    struct StackStringStruct *next;
};
typedef struct StackStringStruct *StackString;

void str_free(StackString);
bool str_equal(StackString, StackString);
StackString str_copy(StackString);
StackString str_ref(StackString);
int str_len(StackString);
char* str_to_str(StackString);
StackString str_from_chr(char);
StackString str_from_str(char *);
StackString str_add(StackString, char);
