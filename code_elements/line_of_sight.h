
#pragma once
#include "../code_core.h"

typedef enum {
    LineT, CircleT, ArcT, EndT
} ObjectType;

typedef enum {
    ReflectT, RefractT, ThroughT, OtherT, OutT, OpaqueT
} BehaviourType;

typedef struct {
    float x1, y1, x2, y2;
} Line;

typedef struct {
    float x, y, r;
} Circle;

typedef struct {
    float x, y, r, a_s, a_e;
} Arc;

typedef struct {
    float in_n;
    float out_n;
} Refract;

typedef union {
    Refract r;
    CoreReturn (* o) (Position pos, float angle, float normal);
    EndCallback e;
} BehaviourElement;

typedef struct {
    ObjectType type;
    union {
        Line l;
        Circle c;
        Arc a;
    } o;
    BehaviourType f_type;
    BehaviourElement f;
    BehaviourType b_type;
    BehaviourElement b;
} RayObject;

float intersect(Position p, float a, RayObject o, Position *iret, float *nret);
CoreReturn ray_trace_through_cell(Position p, float a, RayObject* os, int *count);

CoreReturn through(Position p, float a);
