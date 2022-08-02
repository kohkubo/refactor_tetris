
NAME := tetris
SRCS := $(shell find . -name "*.c")
OBJS := $(SRCS:.c=.o)
DEPS := $(SRCS:.c=.d)
CFLAGS := -Wall -Werror -Wextra -MMD -MP -g -fsanitize=address -fsanitize=undefined
CC := gcc

-include $(DEPS)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lncurses -o $@

clean :
	rm -rf $(OBJS) $(DEPS)

fclean: clean
	rm -f $(NAME)

.PHONY: FORCE