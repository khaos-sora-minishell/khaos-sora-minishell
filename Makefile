NAME		= minishell

OBJ_DIR		= obj

LIBFT_DIR	= libs/libft
GC_DIR		= libs/garbage_collector

LEXER_SRC	= lexer.c lexer_operator.c lexer_word.c lexer_token.c quotes.c lexer_utils.c

PARSER_SRC	= parser.c parse_cmd.c parse_cmd_utils.c parse_cmd_utils2.c build_ast.c build_ast_utils.c

EXPANDER_SRC = expander.c expand_args.c expand_args2.c \
			   expander_utils.c expander_utils2.c expander_utils3.c expander_utils4.c \
			   wildcard.c wildcard_utils.c expand_helpers.c expander_positional.c

EXECUTOR_SRC = executor.c exec_ast.c exec_builtin.c exec_cmd.c \
			   exec_cmd_utils1.c exec_cmd_utils2.c exec_pipe.c redirections.c \
			   here_doc_utils1.c here_doc_utils2.c here_doc_utils3.c here_doc_utils4.c here_doc_manager.c \
			   extra_commands/collor_command.c \
			   extra_commands/extra_commands.c extra_commands/harici_matrix.c \
			   extra_commands/harici_matrix_utils.c extra_commands/pars_vs_executer.c \
			   extra_commands/set_terminal_name.c \
			   extra_commands/tetris.c extra_commands/init_tetris.c extra_commands/movement.c \
			   extra_commands/terminal_utils.c extra_commands/render.c extra_commands/collision.c \
			   extra_commands/lock_piece.c extra_commands/spawn_piece.c extra_commands/cleare_piece.c \
			   extra_commands/calc_speed.c extra_commands/tetromino.c

EXEC_ERROR_SRC = executor_error.c executer_error2.c

BUILTINS_SRC = cd/builtin_cd.c \
			   echo/builtin_echo.c \
			   env/builtin_env.c \
			   exit/builtin_exit.c \
			   export/builtin_export.c export/builtin_expot_utils.c \
			   pwd/builtin_pwd.c \
			   unset/builtin_unset.c \
			   builtin_alias.c builtin_unalias.c \
			   extras/builtin_help.c extras/builtin_true_false.c \
			   extras/builtin_tty.c extras/builtin_type.c extras/source.c

ENV_SRC		= env_manager.c env_manager_utils.c env_manager_utils2.c env_crypto.c env_array.c parse_path.c

SIGNALS_SRC	= signals.c signal_state.c

UTILS_SRC	= error.c ft_atoll.c strings.c strings_utils.c

# Unified build with all features
SRCS = main.c main_utils.c main_utils2.c prompt.c prompt_utils.c history_manager.c history_manager_utils.c config_loader.c config_loader_utils.c \
	   $(addprefix lexer/, $(LEXER_SRC)) \
	   $(addprefix parser/, $(PARSER_SRC)) \
	   $(addprefix expander/, $(EXPANDER_SRC)) \
	   $(addprefix executor/, $(EXECUTOR_SRC)) \
	   $(addprefix executor_error/, $(EXEC_ERROR_SRC)) \
	   $(addprefix builtins/, $(BUILTINS_SRC)) \
	   $(addprefix env/, $(ENV_SRC)) \
	   $(addprefix signals/, $(SIGNALS_SRC)) \
	   $(addprefix utils/, $(UTILS_SRC))

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

LIBFT_LIB		= $(LIBFT_DIR)/libft.a
GC_LIB			= $(GC_DIR)/garbage_collecter.a

CC				= cc
CFLAGS			= -Wall -Wextra -Werror

INCLUDE_FLAGS	= -I. \
				  -I./libs/libft \
				  -I./libs/garbage_collector -I./libs/garbage_collector/include \
				  -I./lexer -I./parser -I./expander -I./executor \
				  -I./executor/extra_commands \
				  -I./executor_error \
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

$(NAME): $(OBJS) $(LIBFT_LIB) $(GC_LIB)
	@echo "$(YELLOW)Linking $(NAME)...$(RESET)"
	@$(CC) $(OBJS) $(LIBFT_LIB) $(GC_LIB) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)✓ Successfully built $(NAME)!$(RESET)"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_LIB):
	@echo "$(YELLOW)Building Libft...$(RESET)"
	@make -s -C $(LIBFT_DIR)
	@echo "$(GREEN)✓ Libft derlendi$(RESET)"

$(GC_LIB):
	@echo "$(YELLOW)Building garbage collector...$(RESET)"
	@make -s -C $(GC_DIR)
	@echo "$(GREEN)✓ Garbage collector derlendi$(RESET)"

bonus: all

clean:
	@$(RM) -r $(OBJ_DIR) 2>/dev/null || true
	@if [ -d "$(LIBFT_DIR)" ]; then make -s -C $(LIBFT_DIR) clean >/dev/null 2>&1 && echo "$(RED)✓ Libft cleaned$(RESET)"; fi
	@if [ -d "$(GC_DIR)" ]; then make -s -C $(GC_DIR) clean >/dev/null 2>&1 && echo "$(RED)✓ Garbage collector cleaned$(RESET)"; fi
	@echo "$(RED)✓ Cleaned object files$(RESET)"

fclean: clean
	@$(RM) $(NAME) 2>/dev/null || true
	@if [ -d "$(LIBFT_DIR)" ]; then make -s -C $(LIBFT_DIR) fclean >/dev/null 2>&1; fi
	@if [ -d "$(GC_DIR)" ]; then make -s -C $(GC_DIR) fclean >/dev/null 2>&1; fi
	@echo "$(RED)✓ Removed binary: $(NAME)$(RESET)"

re: fclean all

debug: CFLAGS += -g
debug: re

.PHONY: all bonus clean fclean re debug