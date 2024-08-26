temp:
	mkdir temp

temp/board.o: temp board_elements/board.c
	gcc -o temp/board.o -c board_elements/board.c 
	
temp/loader.o: temp board_elements/loader.c
	gcc -o temp/loader.o -c board_elements/loader.c 
	
temp/position.o: temp board_elements/position.c
	gcc -o temp/position.o -c board_elements/position.c

temp/cell_behaviours.o: temp code_elements/cell_behaviours.c
	gcc -o temp/cell_behaviours.o -c code_elements/cell_behaviours.c

temp/line_of_sight.o: temp code_elements/line_of_sight.c
	gcc -o temp/line_of_sight.o -c code_elements/line_of_sight.c

temp/opaques.o: temp code_elements/opaques.c
	gcc -o temp/opaques.o -c code_elements/opaques.c

temp/stack_manipulation.o: temp code_elements/stack_manipulation.c
	gcc -o temp/stack_manipulation.o -c code_elements/stack_manipulation.c

temp/stack_element.o: temp stack_elements/stack_element.c
	gcc -o temp/stack_element.o -c stack_elements/stack_element.c

temp/stack_string.o: temp stack_elements/stack_string.c
	gcc -o temp/stack_string.o -c stack_elements/stack_string.c

temp/stack.o: temp stack_elements/stack.c
	gcc -o temp/stack.o -c stack_elements/stack.c

temp/utils.o: temp utils/utils.c
	gcc -o temp/utils.o -c utils/utils.c

temp/bindings.o: temp bindings.c
	gcc -o temp/bindings.o -c bindings.c

temp/code_core.o: temp code_core.c
	gcc -o temp/code_core.o -c code_core.c

temp/pointer.o: temp pointer.c
	gcc -o temp/pointer.o -c pointer.c

see: seeinterpreter.c \
		temp/board.o temp/loader.o temp/position.o \
		temp/cell_behaviours.o temp/line_of_sight.o temp/opaques.o temp/stack_manipulation.o \
		temp/stack_element.o temp/stack_string.o temp/stack.o temp/utils.o \
		temp/bindings.o temp/code_core.o temp/pointer.o
	gcc -o see seeinterpreter.c \
		temp/board.o temp/loader.o temp/position.o \
		temp/cell_behaviours.o temp/line_of_sight.o temp/opaques.o temp/stack_manipulation.o \
		temp/stack_element.o temp/stack_string.o temp/stack.o temp/utils.o \
		temp/bindings.o temp/code_core.o temp/pointer.o -lm

clean:
	rm -r temp see
