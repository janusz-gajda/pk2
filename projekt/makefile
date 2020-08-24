all: release

release : main.c
	gcc -o sudoku main.c src/cJSON/cJSON.c src/generic.c src/json.c src/solving.c src/param.c -lncurses -O2

debug : main.c
	gcc -o sudoku main.c src/cJSON/cJSON.c src/generic.c src/json.c src/solving.c src/param.c -lncurses -fsanitize=address -fno-omit-frame-pointer

clean: 
	rm sudoku