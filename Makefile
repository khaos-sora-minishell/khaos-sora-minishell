# Test Makefile for Lexer Development
# Usage: make -f Makefile_test

NAME = test_lexer
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I./includes -I./libs/garbage_collector -I./libs/ft_printf -I./libs/libft

# Source files
SRCS = test_lexer.c \
       lexer/lexer.c \
       utils/strings.c

# Object files
OBJS = $(SRCS:.c=.o)

# Libraries
LIBFT = libs/libft/libft.a
PRINTF = libs/ft_printf/libftprintf.a
GC = libs/garbage_collector/garbage_collecter.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(PRINTF) $(GC)
	$(CC) $(CFLAGS) $(OBJS) $(PRINTF) $(LIBFT) $(GC) -o $(NAME)
	@echo "✓ Test executable created: ./$(NAME)"

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@make -C libs/libft

$(PRINTF):
	@make -C libs/ft_printf

$(GC):
	@make -C libs/garbage_collector

clean:
	rm -f $(OBJS)
	@make -C libs/libft clean
	@make -C libs/ft_printf clean
	@make -C libs/garbage_collector clean

fclean: clean
	rm -f $(NAME)
	@make -C libs/libft fclean
	@make -C libs/ft_printf fclean
	@make -C libs/garbage_collector fclean

re: fclean all

.PHONY: all clean fclean re
