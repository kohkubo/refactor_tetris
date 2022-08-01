tetris: tetris.c
	gcc -Wall -Werror -Wextra tetris.c -lncurses -o tetris -g -fsanitize=address -fsanitize=undefined