# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/15 00:00:00 by akivam            #+#    #+#              #
#    Updated: 2025/12/15 21:41:39 by akivam           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell
OBJ_DIR		= obj

# Library Directories
LIBFT_DIR	= libs/libft
PRINTF_DIR	= libs/ft_printf
GC_DIR		= libs/garbage_collector

# Source Files by Module
LEXER_SRC	= lexer.c lexer_operator.c lexer_word.c lexer_token.c quotes.c

PARSER_SRC	= parser.c parse_cmd.c build_ast.c

EXPANDER_SRC = expander.c wildcard.c

EXECUTOR_SRC = executor.c exec_ast.c exec_builtin.c exec_cmd.c exec_pipe.c redirections.c

EXEC_ERROR_SRC = executor_error.c

BUILTINS_SRC = builtin_cd.c builtin_echo_utils.c builtin_echo.c builtin_env.c builtin_exit.c \
				builtin_export.c builtin_help.c builtin_pwd.c builtin_true_false.c \
				builtin_tty.c builtin_type.c builtin_unset.c

ENV_SRC		= env_manager.c env_crypto.c env_array.c

EASTER_SRC	= easter_egg.c pars_vs_executer.c set_terminal_name.c

SIGNALS_SRC	= signals.c

UTILS_SRC	= error.c free.c strings.c

# Main sources
SRCS = main.c \
       $(addprefix lexer/, $(LEXER_SRC)) \
       $(addprefix parser/, $(PARSER_SRC)) \
       $(addprefix expander/, $(EXPANDER_SRC)) \
       $(addprefix executor/, $(EXECUTOR_SRC)) \
       $(addprefix executor/easter_egg/, $(EASTER_SRC)) \
       $(addprefix executor_error/, $(EXEC_ERROR_SRC)) \
       $(addprefix builtins/, $(BUILTINS_SRC)) \
       $(addprefix env/, $(ENV_SRC)) \
       $(addprefix signals/, $(SIGNALS_SRC)) \
       $(addprefix utils/, $(UTILS_SRC))

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

# Library Files
LIBFT_LIB		= $(LIBFT_DIR)/libft.a
PRINTF_LIB		= $(PRINTF_DIR)/libftprintf.a
GC_LIB			= $(GC_DIR)/garbage_collecter.a

# Compiler and Flags
CC				= cc
CFLAGS			= -Wall -Wextra -Werror
INCLUDE_FLAGS	= -I./includes -I./libs/libft -I./libs/ft_printf \
				  -I./libs/garbage_collector -I./libs/garbage_collector/include \
				  -I./lexer -I./parser -I./expander -I./executor -I./builtins \
				  -I./env -I./signals -I./utils
CFLAGS			+= -g -g3
CFLAGS			+= $(INCLUDE_FLAGS)
LDFLAGS			= -lreadline -lncurses

RM				= rm -f

# Colors
GREEN			= \033[0;32m
YELLOW			= \033[0;33m
RED				= \033[0;31m
RESET			= \033[0m

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_LIB) $(PRINTF_LIB) $(GC_LIB)
	@echo "$(YELLOW)Linking $(NAME)...$(RESET)"
	@$(CC) $(OBJS) $(LIBFT_LIB) $(PRINTF_LIB) $(GC_LIB) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)✓ Successfully built $(NAME)!$(RESET)"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_LIB):
	@echo "$(YELLOW)Building Libft...$(RESET)"
	@make -s -C $(LIBFT_DIR)

$(PRINTF_LIB):
	@echo "$(YELLOW)Building ft_printf...$(RESET)"
	@make -s -C $(PRINTF_DIR)

$(GC_LIB):
	@echo "$(YELLOW)Building garbage collector...$(RESET)"
	@make -s -C $(GC_DIR)

clean:
	@if [ -d "$(LIBFT_DIR)" ]; then make -s -C $(LIBFT_DIR) clean; fi
	@if [ -d "$(PRINTF_DIR)" ]; then make -s -C $(PRINTF_DIR) clean; fi
	@if [ -d "$(GC_DIR)" ]; then make -s -C $(GC_DIR) clean; fi
	@$(RM) -r $(OBJ_DIR)
	@echo "$(RED)Cleaned object files.$(RESET)"

fclean: clean
	@if [ -d "$(LIBFT_DIR)" ]; then make -s -C $(LIBFT_DIR) fclean; fi
	@if [ -d "$(PRINTF_DIR)" ]; then make -s -C $(PRINTF_DIR) fclean; fi
	@if [ -d "$(GC_DIR)" ]; then make -s -C $(GC_DIR) fclean; fi
	@$(RM) $(NAME)
	@echo "$(RED)Removed binary: $(NAME).$(RESET)"

re: fclean all

debug: CFLAGS += -g
debug: re

valgrind: $(NAME)
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		--suppressions=readline.supp ./$(NAME)

norm:
	@echo "$(YELLOW)Checking norminette...$(RESET)"
	@norminette main.c lexer parser expander executor builtins env signals executor_utils utils includes 2>&1 | grep -v "OK!" || echo "$(GREEN)✓ Norminette OK!$(RESET)"

.PHONY: all clean fclean re debug valgrind norm
