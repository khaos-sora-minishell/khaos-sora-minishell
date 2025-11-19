# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akivam <akivam@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/19 00:00:00 by akivam            #+#    #+#              #
#    Updated: 2025/11/19 00:00:00 by akivam           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

# Compiler and Flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g
INCLUDES	= -I./includes -I./libs/libft -I./libs/ft_printf \
			  -I./libs/grabage_collector -I./libs/get-next-line
LDFLAGS		= -lreadline

# Directories
SRC_DIR		= src
OBJ_DIR		= obj
LIBS_DIR	= libs

# Library Paths
LIBFT_DIR	= $(LIBS_DIR)/libft
PRINTF_DIR	= $(LIBS_DIR)/ft_printf
GC_DIR		= $(LIBS_DIR)/grabage_collector
GNL_DIR		= $(LIBS_DIR)/get-next-line

LIBFT		= $(LIBFT_DIR)/libft.a
PRINTF		= $(PRINTF_DIR)/libftprintf.a
GC			= $(GC_DIR)/libgc.a

# Source Files
SRCS		= $(SRC_DIR)/main/main.c \
			  $(SRC_DIR)/main/signals.c \
			  $(SRC_DIR)/lexer/lexer.c \
			  $(SRC_DIR)/lexer/quotes.c \
			  $(SRC_DIR)/parser/parser.c \
			  $(SRC_DIR)/parser/parse_cmd.c \
			  $(SRC_DIR)/parser/build_ast.c \
			  $(SRC_DIR)/expander/expander.c \
			  $(SRC_DIR)/expander/wildcard.c \
			  $(SRC_DIR)/executor/executor.c \
			  $(SRC_DIR)/executor/exec_cmd.c \
			  $(SRC_DIR)/executor/exec_pipe.c \
			  $(SRC_DIR)/executor/redirections.c \
			  $(SRC_DIR)/builtins/builtin_cd.c \
			  $(SRC_DIR)/builtins/builtin_echo.c \
			  $(SRC_DIR)/builtins/builtin_env.c \
			  $(SRC_DIR)/builtins/builtin_exit.c \
			  $(SRC_DIR)/builtins/builtin_export.c \
			  $(SRC_DIR)/builtins/builtin_pwd.c \
			  $(SRC_DIR)/builtins/builtin_unset.c \
			  $(SRC_DIR)/env/env_list.c \
			  $(SRC_DIR)/env/env_utils.c \
			  $(SRC_DIR)/utils/error.c \
			  $(SRC_DIR)/utils/free.c \
			  $(SRC_DIR)/utils/strings.c

# Object Files
OBJS		= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Colors
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
RED			= \033[0;31m
RESET		= \033[0m

# Rules
all: $(NAME)

$(NAME): $(LIBFT) $(PRINTF) $(GC) $(OBJS)
	@echo "$(YELLOW)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(PRINTF) $(GC) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@echo "$(YELLOW)Building libft...$(RESET)"
	@make -C $(LIBFT_DIR)

$(PRINTF):
	@echo "$(YELLOW)Building ft_printf...$(RESET)"
	@make -C $(PRINTF_DIR)

$(GC):
	@echo "$(YELLOW)Building garbage collector...$(RESET)"
	@make -C $(GC_DIR)

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean
	@make -C $(PRINTF_DIR) clean
	@make -C $(GC_DIR) clean

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@make -C $(PRINTF_DIR) fclean
	@make -C $(GC_DIR) fclean

re: fclean all

# Debugging targets
debug: CFLAGS += -g3 -fsanitize=address
debug: re

valgrind: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		--suppressions=readline.supp ./$(NAME)

# Norminette
norm:
	@echo "$(YELLOW)Checking norminette...$(RESET)"
	@norminette $(SRC_DIR) includes | grep -v "OK!" || echo "$(GREEN)✓ Norminette OK!$(RESET)"

.PHONY: all clean fclean re debug valgrind norm
