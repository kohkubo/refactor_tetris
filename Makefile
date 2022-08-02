tetris: FORCE
	gcc -Wall -Werror -Wextra \
	action.c \
	field.c \
	mino.c \
	tetris.c \
	update_screen.c \
	time.c \
	-lncurses -o tetris \
	-g -fsanitize=address -fsanitize=undefined

.PHONY: FORCE