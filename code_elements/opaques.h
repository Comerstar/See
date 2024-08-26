
#pragma once
#include "../code_core.h"

CoreReturn default_end(Position, float, Board, Bindings, Stack);
CoreReturn output_end(Position, float, Board, Bindings, Stack);
CoreReturn input_end(Position, float, Board, Bindings, Stack);
CoreReturn halt_end(Position, float, Board, Bindings, Stack);

CoreReturn left_end(Position, float, Board, Bindings, Stack);
CoreReturn right_end(Position, float, Board, Bindings, Stack);
CoreReturn up_end(Position, float, Board, Bindings, Stack);
CoreReturn down_end(Position, float, Board, Bindings, Stack);

CoreReturn jump_end(Position, float, Board, Bindings, Stack);

CoreReturn bind_end(Position, float, Board, Bindings, Stack);
CoreReturn write_end(Position, float, Board, Bindings, Stack);
