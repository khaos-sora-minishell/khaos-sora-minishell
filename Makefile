NAME		= minishell
OBJ_DIR		= obj

LIBFT_DIR	= libs/libft
GC_DIR		= libs/garbage_collector

LEXER_SRC	= lexer.c lexer_operator.c lexer_word.c lexer_token.c quotes.c lexer_utils.c

PARSER_SRC	= parser.c parse_cmd.c parse_cmd_utils.c parse_cmd_utils2.c build_ast.c build_ast_utils.c

EXPANDER_SRC = expander.c wildcard.c expand_args.c \
			   expander_utils.c expander_utils2.c expander_utils3.c \
			   wildcard_utils.c expand_helpers.c

EXECUTOR_SRC = executor.c exec_ast.c exec_builtin.c exec_cmd.c \
			   exec_cmd_utils1.c exec_cmd_utils2.c exec_pipe.c redirections.c \
			   here_doc_utils1.c here_doc_utils2.c here_doc_utils3.c here_doc_manager.c

EXEC_ERROR_SRC = executor_error.c executer_error2.c

BUILTINS_SRC = cd/builtin_cd.c \
			   echo/builtin_echo.c \
			   env/builtin_env.c \
			   exit/builtin_exit.c \
			   export/builtin_export.c export/builtin_expot_utils.c \
			   pwd/builtin_pwd.c \
			   unset/builtin_unset.c \
			   builtin_alias_bonus.c builtin_unalias_bonus.c

BONUS_BUILTINS = builtin_alias_bonus.c builtin_unalias_bonus.c

ENV_SRC		= env_manager.c env_manager_utils.c env_manager_utils2.c env_crypto.c env_array.c parse_path.c

SIGNALS_SRC	= signals.c signal_state.c

UTILS_SRC	= error.c ft_atoll.c strings.c strings_utils.c

SRCS = main.c main_utils.c main_utils2.c prompt.c prompt_utils.c history_manager.c config_loader.c config_loader_utils.c \
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
				  -I./executor_error \
				  -I./builtins -I./env -I./signals -I./utils

CFLAGS			+= $(INCLUDE_FLAGS)
LDFLAGS			= -lreadline -lncurses

RM				= rm -f

# Verbose mode (use V=1 for verbose output)
ifeq ($(V),1)
Q =
else
Q = @
endif

# Colors
GREEN			= \033[0;32m
YELLOW			= \033[0;33m
RED				= \033[0;31m
RESET			= \033[0m

all: $(NAME)

$(NAME): .mandatory

.mandatory: $(OBJS) $(LIBFT_LIB) $(GC_LIB)
	$(Q)if [ -f .bonus ]; then \
		echo "$(YELLOW)Linking $(NAME)...$(RESET)"; \
		$(CC) $(OBJS) $(LIBFT_LIB) $(GC_LIB) $(LDFLAGS) -o $(NAME); \
		rm -f .bonus; \
		touch .mandatory; \
		echo "$(GREEN)✓ Successfully built $(NAME)!$(RESET)"; \
	elif [ ! -f .mandatory ]; then \
		echo "$(YELLOW)Linking $(NAME)...$(RESET)"; \
		$(CC) $(OBJS) $(LIBFT_LIB) $(GC_LIB) $(LDFLAGS) -o $(NAME); \
		touch .mandatory; \
		echo "$(GREEN)✓ Successfully built $(NAME)!$(RESET)"; \
	fi

$(OBJ_DIR)/%.o: %.c
	$(Q)mkdir -p $(@D)
	$(Q)echo "$(YELLOW)Compiling $<...$(RESET)"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_LIB):
	$(Q)echo "$(YELLOW)Building Libft...$(RESET)"
	$(Q)make -s -C $(LIBFT_DIR)

$(GC_LIB):
	$(Q)echo "$(YELLOW)Building garbage collector...$(RESET)"
	$(Q)make -s -C $(GC_DIR)


time:
	$(Q)echo "$(YELLOW)=== Mandatory Object Files (obj/) ===$(RESET)"
	$(Q)if [ -d "$(OBJ_DIR)" ]; then \
		find $(OBJ_DIR) -name "*.o" -exec ls -lht {} \; | head -10; \
	else \
		echo "$(RED)No mandatory objects found$(RESET)"; \
	fi
	$(Q)echo "\n$(YELLOW)=== Bonus Object Files (obj_bonus/) ===$(RESET)"
	$(Q)if [ -d "$(BONUS_OBJ_DIR)" ]; then \
		find $(BONUS_OBJ_DIR) -name "*.o" -exec ls -lht {} \; | head -10; \
	else \
		echo "$(RED)No bonus objects found$(RESET)"; \
	fi
	$(Q)echo "\n$(YELLOW)=== Build Markers ===$(RESET)"
	$(Q)ls -lht .mandatory .bonus $(NAME) 2>/dev/null || echo "$(RED)No markers found$(RESET)"

# Bonus: mandatory + bonus features (uses separate obj directory)
BONUS_OBJ_DIR = obj_bonus
BONUS_OBJS = $(addprefix $(BONUS_OBJ_DIR)/, $(SRCS:.c=.o))

$(BONUS_OBJ_DIR)/%.o: %.c
	$(Q)mkdir -p $(@D)
	$(Q)echo "$(YELLOW)Compiling $< (bonus)...$(RESET)"
	$(Q)$(CC) $(CFLAGS) -DBONUS -c $< -o $@

bonus: .bonus

.bonus: $(BONUS_OBJS) $(LIBFT_LIB) $(GC_LIB)
	$(Q)if [ -f .mandatory ]; then \
		echo "$(YELLOW)Linking $(NAME) with bonus...$(RESET)"; \
		$(CC) $(BONUS_OBJS) $(LIBFT_LIB) $(GC_LIB) $(LDFLAGS) -o $(NAME); \
		rm -f .mandatory; \
		touch .bonus; \
		echo "$(GREEN)✓ Built with bonus features!$(RESET)"; \
	elif [ ! -f .bonus ]; then \
		echo "$(YELLOW)Linking $(NAME) with bonus...$(RESET)"; \
		$(CC) $(BONUS_OBJS) $(LIBFT_LIB) $(GC_LIB) $(LDFLAGS) -o $(NAME); \
		touch .bonus; \
		echo "$(GREEN)✓ Built with bonus features!$(RESET)"; \
	fi

clean:
	$(Q)if [ -d "$(LIBFT_DIR)" ]; then make -s -C $(LIBFT_DIR) clean; fi
	$(Q)if [ -d "$(GC_DIR)" ]; then make -s -C $(GC_DIR) clean; fi
	$(Q)$(RM) -r $(OBJ_DIR) obj_bonus
	$(Q)$(RM) .mandatory .bonus
	$(Q)echo "$(RED)Cleaned object files.$(RESET)"

fclean: clean
	$(Q)if [ -d "$(LIBFT_DIR)" ]; then make -s -C $(LIBFT_DIR) fclean; fi
	$(Q)if [ -d "$(GC_DIR)" ]; then make -s -C $(GC_DIR) fclean; fi
	$(Q)$(RM) $(NAME) .mandatory .bonus
	$(Q)echo "$(RED)Removed binary: $(NAME).$(RESET)"

re: fclean all

debug: CFLAGS += -g
debug: re

.PHONY: all clean fclean re debug valgrind norm time bonus