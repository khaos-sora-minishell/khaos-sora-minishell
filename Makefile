# Test Makefile for Lexer Development
# Usage: make or make tester

NAME = test_lexer
TESTER = lexer_tester
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I./includes -I./libs/garbage_collector -I./libs/ft_printf -I./libs/libft

# Source files for basic test
SRCS = test_lexer.c \
       lexer/lexer.c \
       lexer/quotes.c \
       utils/strings.c

# Source files for comprehensive tester
TESTER_SRCS = lexer_tester.c \
              lexer/lexer.c \
              lexer/quotes.c \
              utils/strings.c

# Object files
OBJS = $(SRCS:.c=.o)
TESTER_OBJS = $(TESTER_SRCS:.c=.o)

# Libraries
LIBFT = libs/libft/libft.a
PRINTF = libs/ft_printf/libftprintf.a
GC = libs/garbage_collector/garbage_collecter.a

all: $(NAME)

tester: $(TESTER_OBJS) $(LIBFT) $(PRINTF) $(GC)
	$(CC) $(CFLAGS) $(TESTER_OBJS) $(PRINTF) $(LIBFT) $(GC) -o $(TESTER)
	@echo "✓ Comprehensive tester created: ./$(TESTER)"

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
	rm -f $(OBJS) $(TESTER_OBJS)
	@make -C libs/libft clean
	@make -C libs/ft_printf clean
	@make -C libs/garbage_collector clean

fclean: clean
	rm -f $(NAME) $(TESTER)
	@make -C libs/libft fclean
	@make -C libs/ft_printf fclean
	@make -C libs/garbage_collector fclean

re: fclean all

.PHONY: all tester clean fclean re
