/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:48 by akivam            #+#    #+#             */
/*   Updated: 2026/01/01 21:31:51 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libs/garbage_collector/garbage_collector.h"
# include "libs/libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

# define ENV_TABLE_SIZE 131
# define FNV_PRIME_64 1099511628211UL
# define FNV_OFFSET 14695981039346656037UL
# define XOR_KEY 0x1F

# ifdef BONUS

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
}						t_token_type;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL,
}						t_node_type;

# else

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
}						t_token_type;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
}						t_node_type;

# endif

typedef struct s_env_bucket
{
	char				*key;
	char				*value;
	int					_has_value;
	int					_is_exported;
	struct s_env_bucket	*next;
}						t_env_bucket;

typedef struct s_env_table
{
	t_env_bucket		**buckets;
	int					count;
}						t_env_table;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}						t_token;

typedef struct s_redir
{
	t_token_type		type;
	char				*file;
	char				*delimiter;
	char				*heredoc_tmpfile;
	int					should_expand;
	struct s_redir		*next;
}						t_redir;

typedef struct s_cmd
{
	char				**args;
	t_redir				*redirs;
}						t_cmd;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_ast_node
{
	t_node_type			type;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	struct s_ast_node	*subshell_node;
	t_cmd				*cmd;
}						t_ast_node;

typedef struct s_expand_contex
{
	int					i;
	int					j;
	char				quote;
}						t_expand_contex;

typedef struct s_shell
{
	void				*global_arena;
	void				*cmd_arena;

	char				*terminal_name;
	char				*terminal_name_color;
	char				*terminal_name_bg_color;
	char				*terminal_text_color;
	char				*terminal_bg_color;

	t_env_table			*env_table;
	char				**env_array;
	t_env_table			*alias_table;

	char				**path_dirs;

	int					exit_status;
	t_ast_node			*ast_root;

	int					stdin_backup;
	int					stdout_backup;
	int					redir_stdin_backup;
	int					redir_stdout_backup;
	int					history_fd;
	char				*history_file;
	char				*current_input;

	int					argc;
	char				**argv;
}						t_shell;

t_env_table				*initialize_env_table(char **envp, void *arena);
char					*env_get(t_env_table *table, char *key, void *arena);
void					env_set(t_env_table *table, char *key, char *value,
							void *arena);
void					env_unset(t_env_table *table, char *key);
char					**env_table_to_array(t_env_table *table, void *arena);
void					add_env_entry(t_env_table *table, char *env_str,
							t_gc_context *contex);
void					set_default_shell_vars(t_env_table *table, void *arena);
void					set_default_env_vars(t_env_table *table, void *arena);
void					set_default_prompt_vars(t_env_table *table,
							void *arena);
void					mark_env_as_exported(t_env_table *table, char *key);

unsigned long			fnv1a_hash(char *str);
void					xor_cipher(char *str);

void					init_shell(t_shell *shell, int argc, char **argv,
							char **envp);
void					cleanup_shell(t_shell *shell);
void					cleanup_child_process(t_shell *shell);
char					**parse_path(t_shell *shell);

char					*get_prompt(t_shell *shell);
char					*read_multiline(t_shell *shell);
void					build_prompt_colors(t_shell *shell, char **prompt);
char					*join_lines(char *input, char *next_line);
char					*remove_trailing_newline(char *str);
char					*get_continuation_line(char quote);
void					clean_loop(t_shell *shell);
void					process_input(t_shell *shell, char *input);

char					*find_command_path(char *cmd, t_shell *shell);

void					setup_signals(void);
void					signal_handler(int signum);
void					setup_child_signals(void);
void					ignore_signals(void);
int						get_signal(void);
void					set_signal(int value);
void					reset_signal(void);

void					init_history(t_shell *shell);
void					save_history_file(t_shell *shell);
void					load_shellrc(t_shell *shell);
void					create_shellrc(t_shell *shell);
void					save_to_shellrc(t_shell *shell, char *line);

void					error_exit(char *msg, t_shell *shell);
void					print_error(const char *cmd, const char *arg,
							const char *msg);
void					syntax_error(char *token);
int						command_not_found(char *cmd);

t_token					*lexer(char *input, t_shell *shell);
t_ast_node				*parser(t_token *tokens, t_shell *shell);
t_token					*create_token(t_token_type type, char *value,
							void *arena);
void					add_token(t_token **list, t_token *new_token);
int						is_whitespace(char c);
int						is_special_char(char c);
t_ast_node				*build_ast(t_token *tokens, t_shell *shell);

int						match_pattern(char *pattern, char *str);
int						count_matches(DIR *directory, char *pattern);
void					add_matches(DIR *directory, char *pattern,
							char **result, t_shell *shell);
void					sort_strings(char **strings, int count);

int						has_quotes(char *str);
char					*expand_string(char *str, t_shell *shell);
void					executor_run(t_shell *shell);
int						add_expanded_results(char ***result, int *idx,
							char **wildcard_res);
int						count_args(char **args);
char					**expand_wildcard(char *pattern, t_shell *shell);
char					**expand_args(char **args, t_shell *shell);
char					*process_dollar(char *str, int *i, t_shell *shell);
char					*process_tilde(char *str, int *i, t_shell *shell);

char					**split_args(char *str, void *arena);

#endif
