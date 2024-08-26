
#pragma once
#include <stdbool.h>

typedef struct {int x, y;} Cell;

typedef struct {Cell cell; float f_x, f_y;} Position;

Position pos_from_cell(Cell);
Position next_cell(Position, float);
bool cell_equal(Cell, Cell);
float full_position_x(Position);
float full_position_y(Position);
Position from_full_position(Cell, float, float);
