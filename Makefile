NAME		= minishell
OBJ_DIR		= obj

LIBFT_DIR	= libs/libft
PRINTF_DIR	= libs/ft_printf
GC_DIR		= libs/garbage_collector
GNL_DIR		= libs/get-next-line

LEXER_SRC	= lexer.c lexer_operator.c lexer_word.c lexer_token.c quotes.c

PARSER_SRC	= parser.c parse_cmd.c parse_cmd_utils.c build_ast.c

EXPANDER_SRC = expander.c wildcard.c expand_args.c \
			   expander_utils.c wildcard_utils.c expand_helpers.c

EXECUTOR_SRC = executor.c exec_ast.c exec_builtin.c exec_cmd.c \
			   exec_cmd_utils.c exec_pipe.c redirections.c \
			   here_doc_utils.c here_doc_manager.c

EXEC_ERROR_SRC = executor_error.c

BUILTINS_SRC = cd/builtin_cd.c \
			   echo/builtin_echo.c echo/builtin_echo_utils.c \
			   env/builtin_env.c \
			   exit/builtin_exit.c \
			   export/builtin_export.c export/builtin_expot_utils.c \
			   pwd/builtin_pwd.c \
			   unset/builtin_unset.c \
			   extras/builtin_help.c extras/builtin_true_false.c \
			   extras/builtin_tty.c extras/builtin_type.c

ENV_SRC		= env_manager.c env_manager_utils.c env_crypto.c env_array.c parse_path.c

EASTER_SRC	= easter_egg.c pars_vs_executer.c set_terminal_name.c harici_matrix.c

SIGNALS_SRC	= signals.c signal_state.c

UTILS_SRC	= error.c strings.c strings_utils.c

GNL_SRC		= get_next_line.c get_next_line_utils.c

SRCS = main.c history_manager.c config_loader.c \
	   $(addprefix lexer/, $(LEXER_SRC)) \
	   $(addprefix parser/, $(PARSER_SRC)) \
	   $(addprefix expander/, $(EXPANDER_SRC)) \
	   $(addprefix executor/, $(EXECUTOR_SRC)) \
	   $(addprefix executor/easter_egg/, $(EASTER_SRC)) \
	   $(addprefix executor_error/, $(EXEC_ERROR_SRC)) \
	   $(addprefix builtins/, $(BUILTINS_SRC)) \
	   $(addprefix env/, $(ENV_SRC)) \
	   $(addprefix signals/, $(SIGNALS_SRC)) \
	   $(addprefix utils/, $(UTILS_SRC)) \
	   $(addprefix $(GNL_DIR)/, $(GNL_SRC))

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

LIBFT_LIB		= $(LIBFT_DIR)/libft.a
PRINTF_LIB		= $(PRINTF_DIR)/libftprintf.a
GC_LIB			= $(GC_DIR)/garbage_collecter.a

CC				= cc
CFLAGS			= -Wall -Wextra -Werror

INCLUDE_FLAGS	= -I. \
				  -I./libs/libft -I./libs/ft_printf \
				  -I./libs/garbage_collector -I./libs/garbage_collector/include \
				  -I./libs/get-next-line \
				  -I./lexer -I./parser -I./expander -I./executor \
				  -I./executor/easter_egg -I./executor_error \
				  -I./builtins -I./env -I./signals -I./utils

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
	@norminette main.c history_manager.c config_loader.c lexer parser expander executor \
		builtins env signals executor_utils utils includes 2>&1 | grep -v "OK!" || echo "$(GREEN)✓ Norminette OK!$(RESET)"

easteregg: CFLAGS += -DEASTEREGG
easteregg: re
	@echo "$(GREEN)✓ Built with easter egg features!$(RESET)"

.PHONY: all clean fclean re debug valgrind norm easteregg