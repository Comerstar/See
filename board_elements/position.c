
#include <math.h>
#include "position.h"

Position pos_from_cell(Cell c) {
    return (Position){c, 0, 0};
}

Position next_cell(Position p, float a) {
    float cosa = cos(a), sina = sin(a);
    if (cosa == 0) {
        p.cell.y += sina > 0 ? 1 : -1;
        p.f_y = sina > 0 ? -0.5f : 0.5f;
        
        return p;
    }
    else if (sina == 0) {
        p.cell.x += cosa > 0 ? 1 : -1;
        p.f_x = cosa > 0 ? -0.5f : 0.5f;
        
        return p;
    }
    float x_dist = cosa < 0 ? -0.5f - p.f_x : 0.5f - p.f_x;
    float y_dist = sina < 0 ? -0.5f - p.f_y : 0.5f - p.f_y;
    float x_fact = x_dist / cosa;
    float y_fact = y_dist / sina;

    if (x_fact < y_fact) {
        p.cell.x += cosa > 0 ? 1 : -1;
        p.f_x = cosa > 0 ? -0.5f : 0.5f;
        p.f_y += x_dist * sina / cosa;
        return p;
    }
    else if (x_fact == y_fact) {
        p.cell.x += cosa > 0 ? 1 : -1;
        p.cell.y += sina > 0 ? 1 : -1;
        p.f_x = cosa > 0 ? -0.5f : 0.5f;
        p.f_y = sina > 0 ? -0.5f : 0.5f;
        return p;
    }
    else {
        p.cell.y += sina > 0 ? 1 : -1;
        p.f_y = sina > 0 ? -0.5f : 0.5f;
        p.f_x += y_dist * cosa / sina;
        return p;
    }
}

bool cell_equal(Cell c1, Cell c2) {
    return c1.x == c2.x && c1.y == c2.y;
}

float full_position_x(Position p) {
    return p.cell.x + p.f_x;
}

float full_position_y(Position p) {
    return p.cell.y + p.f_y;
}

Position from_full_position(Cell c, float x, float y) {
    return (Position){c, x - c.x, y - c.y};
}
