NAME := tetris

CC := clang
CFLAGS := -Wall -Werror -Wextra -MMD -MP -O2 -pthread

SRCDIR := .
OBJDIR := ./obj
SRCS := $(shell find . -name "*.c")
OBJS := $(SRCS:$(SRCDIR)%.c=$(OBJDIR)%.o)
DEPS := $(SRCS:$(SRCDIR)%.c=$(OBJDIR)%.d)

all : $(NAME)

-include $(DEPS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)/$(*D)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lncurses -o $@

clean :
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug: CFLAGS +=  -g -fsanitize=undefined -fsanitize=thread
debug: re

.PHONY: FORCE