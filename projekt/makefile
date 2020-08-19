all: release

release : main.c
	gcc -o main main.c -lncurses -O2

debug : main.c
	gcc -o main main.c -lncurses -fsanitize=address -fno-omit-frame-pointer

clean: 
	rm main