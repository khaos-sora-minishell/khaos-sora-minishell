NAME		= minishell
OBJ_DIR		= obj

LIBFT_DIR	= libs/libft
GC_DIR		= libs/garbage_collector

LEXER_SRC	= lexer.c lexer_operator.c lexer_word.c lexer_token.c quotes.c lexer_utils.c

PARSER_SRC	= parser.c parse_cmd.c parse_cmd_utils.c parse_cmd_utils2.c build_ast.c build_ast_utils.c

EXPANDER_SRC = expander.c expand_args.c expand_args2.c \
			   expander_utils.c expander_utils2.c expander_utils3.c expander_utils4.c \
			   expand_helpers.c expander_positional.c wildcard.c wildcard_utils.c

EXECUTOR_SRC = executor.c exec_ast.c exec_builtin.c exec_cmd.c \
			   exec_cmd_utils1.c exec_cmd_utils2.c exec_pipe.c redirections.c \
			   here_doc_utils1.c here_doc_utils2.c here_doc_utils3.c here_doc_utils4.c here_doc_manager.c \
			   extra_commands/collor_command.c \
			   extra_commands/color_utils.c \
			   extra_commands/prompt_utils.c \
			   extra_commands/extra_commands.c \
			   extra_commands/pars_vs_executer.c \
			   extra_commands/set_terminal_name.c \
			   extra_commands/tetris/calc_speed.c \
			   extra_commands/tetris/cleare_piece.c \
			   extra_commands/tetris/collision.c \
			   extra_commands/tetris/init_tetris.c \
			   extra_commands/tetris/input.c \
			   extra_commands/tetris/lock_piece.c \
			   extra_commands/tetris/movement.c \
			   extra_commands/tetris/render_board.c \
			   extra_commands/tetris/render_game_over.c \
			   extra_commands/tetris/render_utils.c \
			   extra_commands/tetris/render.c \
			   extra_commands/tetris/spawn_piece.c \
			   extra_commands/tetris/start_tetris.c \
			   extra_commands/tetris/tetris.c \
			   extra_commands/tetris/tetromino.c

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
			   extras/builtin_tty.c extras/builtin_type.c

ENV_SRC		= env_manager.c env_manager_utils.c env_manager_utils2.c env_crypto.c env_array.c parse_path.c

SIGNALS_SRC	= signals.c signal_state.c

UTILS_SRC	= error.c ft_atoll.c strings.c strings_utils.c

SRCS = main.c main_utils.c main_utils2.c prompt.c prompt_utils.c history_manager.c history_manager_utils.c \
	   config_loader.c config_loader_utils.c \
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
CFLAGS			= -Wall -Wextra -Werror -g -DBONUS

INCLUDE_FLAGS	= -I. \
				  -I./libs/libft \
				  -I./libs/garbage_collector -I./libs/garbage_collector/include \
				  -I./lexer -I./parser -I./expander -I./executor \
				  -I./executor/extra_commands \
				  -I./executor/extra_commands/tetris \
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
	@echo "$(GREEN)✓ Successfully built $(NAME) with all features!$(RESET)"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_LIB):
	@echo "$(YELLOW)Building Libft...$(RESET)"
	@make -s -C $(LIBFT_DIR)

$(GC_LIB):
	@echo "$(YELLOW)Building garbage collector...$(RESET)"
	@make -s -C $(GC_DIR)

clean:
	@if [ -d "$(LIBFT_DIR)" ]; then make -s -C $(LIBFT_DIR) clean; fi
	@if [ -d "$(GC_DIR)" ]; then make -s -C $(GC_DIR) clean; fi
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@if [ -d "$(LIBFT_DIR)" ]; then make -s -C $(LIBFT_DIR) fclean; fi
	@if [ -d "$(GC_DIR)" ]; then make -s -C $(GC_DIR) fclean; fi
	@$(RM) $(NAME)
	@echo "$(RED)Removed binary: $(NAME).$(RESET)"

re: fclean all

debug: CFLAGS += -g
debug: re

.PHONY: all clean fclean re debug