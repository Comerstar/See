
#include "line_of_sight.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <stdlib.h>
#include <stdio.h>
#include "../settings.h"

float reflect_angle_in_line_at_angle(float i, float n) {
    //printf("i: %f, n: %f\n", i, n);
    return fmod(2 * n - i - M_PI, 2 * M_PI);
}

bool angle_between(float a, float min_a, float max_a) {
    while (max_a - 2 * M_PI > min_a) {
        max_a -= 2 * M_PI;
    }
    while (max_a < min_a) {
        max_a += 2 * M_PI;
    }
    while (a > max_a) {
        a -= 2 * M_PI;
    }
    while (a < min_a) {
        a += 2 * M_PI;
    }
    return a <= max_a;
}

CoreReturn through(Position p, float a) {
    return (CoreReturn){0, p, a};
}

float intersect(Position p, float a, RayObject o, Position *iret, float *nret) {
    float c = cosf(a), s = sinf(a);
    //printf("c: %f s: %f\n", c, s);
    float ac, bc, cc, det;
    switch (o.type) {
        case LineT:
            // Solve for t & k:
            // x + k cos(a) = (1 - t) x1 + t x2
            // y + k sin(a) = (1 - t) y1 + t y2
            Line l = o.o.l;
            //printf("x1: %f y1: %f x2: %f y2: %f\n", l.x1, l.y1, l.x2, l.y2);
            float m = tanf(a), num = m*l.x1 - m*p.f_x - l.y1 + p.f_y, den = m*l.x1 - m*l.x2 - l.y1 + l.y2;
            if (den != 0) {
                float t = num / den;
                //printf("t: %f x: %f y: %f\n", t, (1 - t)*l.x1 + t*l.x2, (1 - t)*l.y1 + t*l.y2);
                if (0 <= t && t <= 1) {
                    // We now need to ensure k is positive
                    float k;
                    if (fabsf(c) > fabsf(s)) {
                        k = ((1 - t)*l.x1 + t*l.x2 - p.f_x) / c;
                    } else {
                        k = ((1 - t)*l.y1 + t*l.y2 - p.f_y) / s;
                    }
                    //printf("k: %f x: %f y: %f\n", k, p.f_x + k*c, p.f_y + k*s);
                    if (k >= 0) {
                        *iret = (Position){p.cell, p.f_x + k*c, p.f_y + k*s};
                        *nret = atan2(l.x2 - l.x1, l.y1 - l.y2);
                        //printf("Normal angle: %f\n", *nret);
                        return k;
                    }
                }
            }
            break;
        case CircleT:
            // Solve for k
            // (k cos(a) + p.f_x - xc) ^ 2 + (k sin(a) + p.f_y - yc) ^ 2 = r^2
            // (cos(a)^2 + sin(a)^2) * k^2 - 2*(xc*cos(a) + yc*sin(a)) * k + xc^2 + yc^2 - r^2 = 0
            Circle circ = o.o.c;
            ac = c*c + s*s, bc = 2*((p.f_x - circ.x)*c + (p.f_y - circ.y)*s), 
                    cc = (p.f_x - circ.x)*(p.f_x - circ.x) + (p.f_y - circ.y)*(p.f_y - circ.y) - circ.r*circ.r;
            det = bc * bc - 4 * ac * cc;
            if (det >= 0) {
                float k = (-bc - sqrtf(det)) / (2 * ac);
                //printf("k1: %f x: %f y: %f\n", k, p.f_x + k*c, p.f_y + k*s);
                if (k >= 0) {
                    *iret = (Position){p.cell, p.f_x + k*c, p.f_y + k*s};
                    *nret = atan2(p.f_y + k*s - circ.y, p.f_x + k*c - circ.x);
                    return k;
                } else {
                    k = (-bc + sqrtf(det)) / (2 * ac);
                    //printf("k2: %f x: %f y: %f\n", k, p.f_x + k*c, p.f_y + k*s);
                    if (k >= 0) {
                        *iret = (Position){p.cell, p.f_x + k*c, p.f_y + k*s};
                        *nret = atan2(p.f_y + k*s - circ.y, p.f_x + k*c - circ.x);
                        return k;
                    }
                }
            }
            break;
        case ArcT:
            Arc arc = o.o.a;
            ac = c*c + s*s, bc = -2*(arc.x*c + arc.y*s), cc = arc.x*arc.x + arc.y*arc.y - arc.r*arc.r;
            det = bc * bc - 4 * ac * cc;
            if (det >= 0) {
                float k = (-bc - sqrtf(det)) / (2 * ac);
                float theta = atan2(k*s - arc.y, k*c - arc.x);
                if (k >= 0 && angle_between(theta, arc.a_e, arc.a_s)) {
                    *iret = (Position){p.cell, p.f_x + k*c, p.f_y + k*s};
                    *nret = atan2(p.f_y + k*s - circ.y, p.f_x + k*c - circ.x);
                    return k;
                } else {
                    k = (-bc + sqrtf(det)) / (2 * ac);
                    theta = atan2(k*s - arc.y, k*c - arc.x);
                    if (k >= 0 && angle_between(theta, arc.a_e, arc.a_s)) {
                        *iret = (Position){p.cell, p.f_x + k*c, p.f_y + k*s};
                        *nret = atan2(p.f_y + k*s - circ.y, p.f_x + k*c - circ.x);
                        return k;
                    }
                }
            }
            break;
    }
    return INFINITY;
}

float angle_dot(float a, float n) {
    return cosf(a)*cosf(n) + sinf(a)*sinf(n);
}

struct RayObjectListCellStruct {
    RayObject e;
    int i;
    bool front;
    Position intersect;
    float normal;
    struct RayObjectListCellStruct *next;
};
typedef struct RayObjectListCellStruct *RayObjectListCell;

struct RayObjectListStruct {
    RayObjectListCell l;
    int length;
};
typedef struct RayObjectListStruct *RayObjectList;

void o_list_free(RayObjectList l) {
    RayObjectListCell c = l->l, n;
    while (c) {
        n = c->next;
        free(c);
        c = n;
    }
    free(l);
}

void o_list_clear(RayObjectList l) {
    RayObjectListCell c = l->l, n;
    while (c) {
        n = c->next;
        free(c);
        c = n;
    }
    l->length = 0;
    l->l = 0;
}

RayObjectList o_list_new(void) {
    RayObjectList ret = malloc(sizeof(struct RayObjectListStruct));
    *ret = (struct RayObjectListStruct){0, 0};
    return ret;
}

void o_list_add(RayObjectList l, RayObject o, int i, bool f, Position p, float n) {
    RayObjectListCell c = malloc(sizeof(struct RayObjectListCellStruct));
    *c = (struct RayObjectListCellStruct){o, i, f, p, n, l->l};
    l->l = c;
    l->length++;
}

bool o_list_contains_index(RayObjectList l, int i) {
    RayObjectListCell c = l->l;
    while(c) {
        //printf("Checking c->i, %d against i, %i\n", c->i, i);
        if (c->i == i) {
            return true;
        }
        c = c->next;
    }
    return false;
}

int o_list_len(RayObjectList l) {
    int i = 0;
    RayObjectListCell c = l->l;
    while (c) {
        i++;
        c = c->next;
    }
    return i;
}

void o_list_exchange(RayObjectList l1, RayObjectList l2) {
    int temp_l = l1->length;
    RayObjectListCell temp_c = l1 -> l;
    l1->length = l2->length;
    l1->l = l2->l;
    l2->length = temp_l;
    l2->l = temp_c;
}

CoreReturn ray_trace_through_cell(Position p, float a, RayObject* os, int *count) {
    Position point = p, temp_p;
    float angle = a, min_k, temp_norm, k;
    bool temp_front;
    float dot;
    int i;
    if(count) *count = 0;
    RayObjectList last = o_list_new(), next = o_list_new();
    BehaviourElement be;
    while (1) {
        //printf("\nIntersecting x %f y %f a %f\n", point.f_x, point.f_y, angle);
        min_k = INFINITY;
        for (i = 0; os[i].type != EndT; i++) {
            //printf("%d\n", i);
            k = intersect(point, angle, os[i], &temp_p, &temp_norm);
            dot = angle_dot(angle, temp_norm);
            temp_front = angle_dot <= 0;
            if ((-EPSILON > dot || dot > EPSILON) && 0<= k && k <= min_k && (temp_front ? os[i].f_type : os[i].b_type) != ThroughT && !o_list_contains_index(last, i)) {
                if (k < min_k) {
                    min_k = k;
                    //printf("Clearing\n");
                    o_list_clear(next);
                    //printf("Cleared\n");
                    o_list_add(next, os[i], i, temp_front, temp_p, temp_norm);
                    //printf("Added\n");
                } else {
                    min_k = k;
                    //printf("Adding\n");
                    o_list_add(next, os[i], i, temp_front, temp_p, temp_norm);
                    //printf("Added\n");
                }
            }
        }
        //printf("Finished intersecting with %d items and %d actual items and min_k %f\n", next->length, o_list_len(next), min_k);
        if (min_k == INFINITY || next->length == 0) {
            break;
        } else {
            RayObjectListCell c = next->l;
            //i = 0;
            while (c) {
                if(count) (*count)++;
                //printf("Dealing with item %d\n", i++);
                be = c->front ? c->e.f : c->e.b;
                //printf("Behaviour %d\n", c->front ? c->e.f_type : c->e.b_type);
                switch (c->front ? c->e.f_type : c->e.b_type) {
                    case ReflectT:
                        //printf("Reflecting in angle %f\n", c->normal);
                        point = c->intersect;
                        angle = reflect_angle_in_line_at_angle(angle, c->normal);
                        break;
                    case RefractT:
                        Refract rfct = be.r;
                        point = c->intersect;
                        break;
                    case ThroughT:
                        break;
                    case OtherT:
                        CoreReturn temp_ret = be.o(c->intersect, angle, temp_norm);
                        if (temp_ret.callback) {
                            o_list_free(last);
                            o_list_free(next);
                            return temp_ret;
                        } else {
                            point = temp_ret.pos;
                            angle = temp_ret.a;
                        }
                        break;
                    case OutT:
                        o_list_free(last);
                        o_list_free(next);
                        return (CoreReturn){0, point, angle};
                    case OpaqueT:
                        o_list_free(last);
                        o_list_free(next);
                        return (CoreReturn){be.e, point, angle};
                }
                c = c->next;
            }
        }
        o_list_exchange(last, next);
        o_list_clear(next);
    }
    o_list_free(last);
    o_list_free(next);
    return (CoreReturn){0, point, angle};
}
