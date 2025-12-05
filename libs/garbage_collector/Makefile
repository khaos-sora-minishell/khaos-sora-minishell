NAME = garbage_collector.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude
AR = ar
RM = rm -f

# ANSI color codes
GREEN := \033[0;32m
YELLOW := \033[1;33m
BLUE := \033[1;34m
RED := \033[0;31m
RESET := \033[0m

SRC_DIR = src
OBJ_DIR = obj

SRC = memory/gc_malloc.c \
      memory/gc_calloc.c \
      memory/gc_realloc.c \
      memory/gc_free.c \
      collector/gc_collect.c \
      collector/gc_mark.c \
      collector/gc_sweep.c \
      lifecycle/gc_create.c \
      lifecycle/gc_destroy.c \
      scope/gc_scope_push.c \
      scope/gc_scope_pop.c \
      scope/gc_scope_pop_all.c \
      string/gc_strlen.c \
      string/gc_strcmp.c \
      string/gc_strncmp.c \
      string/gc_strchr.c \
      string/gc_strrchr.c \
      string/gc_strdup.c \
      string/gc_strndup.c \
      string/gc_strjoin.c \
      string/gc_atoi.c \
      string/gc_itoa.c \
      string/gc_uitoa.c \
      string/gc_substr.c \
      string/gc_strtrim.c \
      utils/gc_bzero.c \
      utils/memory_utils.c \
      utils/gc_find_allocation.c \
      config/gc_set_mode.c \
      config/gc_get_mode.c \
      config/gc_set_debug.c \
      stats/gc_get_stats.c \
      stats/gc_print_stats.c \
      wrapper/gc_wrapper.c

SRCS = $(addprefix $(SRC_DIR)/,$(SRC))
OBJS = $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf '$(GREEN)Compiling %s...$(RESET)\n' "$<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@printf '$(YELLOW)Creating static library %s$(RESET)\n' "$(NAME)"
	@$(AR) -rcs $(NAME) $(OBJS)

clean:
	@$(RM) $(OBJS)
	@$(RM) -r $(OBJ_DIR)
	@printf '$(RED)Cleaned object files.$(RESET)\n'

fclean: clean
	@$(RM) $(NAME)
	@printf '$(RED)Cleaned library files.$(RESET)\n'

re: fclean all

info:
	@printf '\n$(BLUE)══════════════════════════════════════════════════════════════$(RESET)\n'
	@printf '$(YELLOW)To integrate into your legacy projects, add to Makefile:$(RESET)\n\n'
	@printf '  GC_LIB = path/to/garbage_collecter.a\n'
	@printf '  GC_WRAP = -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=free\n\n'
	@printf '  $$(NAME): $$(OBJS) $$(GC_LIB)\n'
	@printf '  	$$(CC) $$(OBJS) $$(GC_LIB) $$(GC_WRAP) -o $$(NAME)\n\n'
	@printf '$(BLUE)══════════════════════════════════════════════════════════════$(RESET)\n\n'

.PHONY: all clean fclean re info info