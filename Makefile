
temp/board.o:
	gcc -o temp/board.o -c board_elements/board.c 
	
temp/loader.o:
	gcc -o temp/loader.o -c board_elements/loader.c 
	
temp/position.o:
	gcc -o temp/position.o -c board_elements/position.c

temp/cell_behaviours.o:
	gcc -o temp/cell_behaviours.o -c code_elements/cell_behaviours.c

temp/line_of_sight.o:
	gcc -o temp/line_of_sight.o -c code_elements/line_of_sight.c

temp/opaques.o:
	gcc -o temp/opaques.o -c code_elements/opaques.c

temp/stack_manipulation.o:
	gcc -o temp/stack_manipulation.o -c code_elements/stack_manipulation.c

temp/stack_element.o:
	gcc -o temp/stack_element.o -c stack_elements/stack_element.c

temp/stack_string.o:
	gcc -o temp/stack_string.o -c stack_elements/stack_string.c

temp/stack.o:
	gcc -o temp/stack.o -c stack_elements/stack.c

temp/utils.o:
	gcc -o temp/utils.o -c utils/utils.c

temp/bindings.o:
	gcc -o temp/bindings.o -c bindings.c

temp/code_core.o:
	gcc -o temp/code_core.o -c code_core.c

temp/pointer.o:
	gcc -o temp/pointer.o -c pointer.c

see: temp/board.o temp/loader.o temp/position.o \
		temp/cell_behaviours.o temp/line_of_sight.o temp/opaques.o temp/stack_manipulation.o \
		temp/stack_element.o temp/stack_string.o temp/stack.o temp/utils.o \
		temp/bindings.o temp/code_core.o temp/pointer.o
	gcc -o see seeinterpreter.c \
		temp/board.o temp/loader.o temp/position.o \
		temp/cell_behaviours.o temp/line_of_sight.o temp/opaques.o temp/stack_manipulation.o \
		temp/stack_element.o temp/stack_string.o temp/stack.o temp/utils.o \
		temp/bindings.o temp/code_core.o temp/pointer.o -lm

clean:
	rm temp/bindings.o temp/board.o temp/cell_behaviours.o temp/code_core.o temp/line_of_sight.o temp/loader.o temp/opaques.o \
		temp/pointer.o temp/position.o temp/stack_element.o temp/stack_manipulation.o temp/stack_string.o temp/stack.o temp/utils.o \
		see
