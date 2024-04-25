
#include "stack_string.h"
#include <stdlib.h>

void str_free(StackString s) {
    StackString next;
    while(s) {
        s->ref_count--;
        if (!s->ref_count) {
            next = s -> next;
            free(s);
            s = next;
        } else break;
    }
}

bool str_equal(StackString s1, StackString s2) {
    if (s1 && s2) {
        if (s1->c == s2->c) return str_equal(s1->next, s2->next);
        else return false;
    } 
    else if (s1 || s2) return false;
    else return true;
}

StackString str_copy(StackString s) {
    if (s) {
        StackString rest = str_copy(s->next);
        StackString next = malloc(sizeof(struct StackStringStruct));
        *next = (struct StackStringStruct){s->c, 1, rest};
        return next;
    } else {
        return s;
    }
}

StackString str_ref(StackString s) {
    s->ref_count++;
    return s;
}

int str_len(StackString s) {
    int i = 0;
    while(s) {
        i++;
        s = s->next;
    }
    return i;
}

char* str_to_str(StackString s) {
    int len = str_len(s);
    char* ret = malloc(sizeof(char) * (len + 1));
    int i = 0;
    while(s) {
        ret[len - 1 - i] = s->c;
        s = s->next;
        i++;
    }
    ret[len] = '\0';
    return ret;
}

StackString str_from_chr(char c) {
    StackString ret = malloc(sizeof(struct StackStringStruct));
    *ret = (struct StackStringStruct){c, 1, 0};
    return ret;
}

StackString str_from_str(char* str) {
    StackString ret = 0, next;
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        next = malloc(sizeof(struct StackStringStruct));
        *next = (struct StackStringStruct){str[i], 1, ret};
        ret = next;
    }
    return ret;
}

StackString str_add(StackString s, char c) {
    StackString ret = malloc(sizeof(struct StackStringStruct));
    if (s) {
        *ret = (struct StackStringStruct){c, 1, str_ref(s)};
    } else {
        *ret = (struct StackStringStruct){c, 1, 0};
    }
    return ret;
}
