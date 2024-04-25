
#pragma once
#include "../code_core.h"

enum ObjectTypeEnum {
    LineT, CircleT, ArcT, EndT
};
typedef enum ObjectTypeEnum ObjectType;

enum BehaviourTypeEnum {
    ReflectT, RefractT, ThroughT, OtherT, OutT, OpaqueT
};
typedef enum BehaviourTypeEnum BehaviourType;

struct LineStruct {
    float x1, y1, x2, y2;
};
typedef struct LineStruct Line;

struct CircleStruct {
    float x, y, r;
};
typedef struct CircleStruct Circle;

struct ArcStruct {
    float x, y, r, a_s, a_e;
};
typedef struct ArcStruct Arc;

struct RefractStruct {
    float in_n;
    float out_n;
};
typedef struct RefractStruct Refract;

typedef union {
        Refract r;
        CoreReturn (* o) (Position pos, float angle, float normal);
        EndCallback e;
    } BehaviourElement;

struct RayTracingStruct {
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
};
typedef struct RayTracingStruct RayObject;

float intersect(Position p, float a, RayObject o, Position *iret, float *nret);
CoreReturn ray_trace_through_cell(Position p, float a, RayObject* os, int *count);

CoreReturn through(Position p, float a);
