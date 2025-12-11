/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:48 by akivam            #+#    #+#             */
/*   Updated: 2025/12/12 00:07:10 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ========================================================================== */
/*                               INCLUDES                                     */
/* ========================================================================== */

# include "garbage_collector.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

/* ========================================================================== */
/*                          GLOBAL SIGNAL VARIABLE                            */
/* ========================================================================== */

extern volatile sig_atomic_t	g_signal;

/* ========================================================================== */
/*                            ENUMERATIONS                                    */
/* ========================================================================== */

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
}								t_token_type;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL,
}								t_node_type;

/* ========================================================================== */
/*                           DATA STRUCTURES                                  */
/* ========================================================================== */

typedef struct s_token
{
	t_token_type				type;
	char						*value;
	struct s_token				*next;
}								t_token;

typedef struct s_redir
{
	t_token_type				type;
	char						*file;
	char						*delimiter;
	char						*heredoc_tmpfile;
	struct s_redir				*next;
}								t_redir;

typedef struct s_cmd
{
	char						**args;
	t_redir						*redirs;
}								t_cmd;

typedef struct s_env
{
	char						*key;
	char						*value;
	struct s_env				*next;
}								t_env;

typedef struct s_ast_node
{
	t_node_type					type;

	struct s_ast_node			*left;
	struct s_ast_node			*right;

	struct s_ast_node			*subshell_node;

	t_cmd						*cmd;
}								t_ast_node;

typedef struct s_shell
{
	char						*terminal_name;
	char						*terminal_text_color;
	char						*terminal_bg_color;

	void						*global_arena;
	void						*cmd_arena;

	t_env						*env_list;
	char						**env_array;
	t_env						*alias_list;

	char						**path_dirs;

	int							exit_status;
	t_ast_node					*ast_root;

	int							stdin_backup;
	int							stdout_backup;
}								t_shell;

/* ========================================================================== */
/*                        FUNCTION PROTOTYPES                                 */
/* ========================================================================== */

/* ========== MAIN ========== */
int								main(int ac, char **av, char **envp);
void							init_shell(t_shell *shell, char **envp);
void							cleanup_shell(t_shell *shell);

/* ========== ENVIRONMENT ========== */
t_env							*init_env(char **envp, void *arena);
char							*get_env_value(t_env *env, char *key);
void							set_env_value(t_env **env, char *key,
									char *value, void *arena);
void							unset_env_value(t_env **env, char *key);
char							**env_list_to_array(t_env *env, void *arena);
t_env							*env_array_to_list(char **envp, void *arena);
void							free_env_list(t_env *env);
void							update_env_array(t_shell *shell);

/* ========== PATH HANDLING ========== */
char							*find_command_path(char *cmd, t_shell *shell);
char							**parse_path(t_env *env, void *arena);

/* ========== SIGNALS ========== */
void							setup_signals(void);
void							signal_handler(int signum);
void							setup_child_signals(void);
void							ignore_signals(void);

/* ========== ERROR HANDLING ========== */
void							error_exit(char *msg, t_shell *shell);
void							print_error(char *cmd, char *msg);
void							syntax_error(char *token);
int								command_not_found(char *cmd);

/* ========== LEXER ========== */
t_token							*lexer(char *input, t_shell *shell);
int								handle_quotes(char *str, int *i);
t_token							*create_token(t_token_type type, char *value,
									void *arena);
void							add_token(t_token **list, t_token *new_token);

/* ========== PARSER ========== */
t_ast_node						*parser(t_token *tokens, t_shell *shell);
t_cmd							*parse_simple_command(t_token **tokens,
									t_shell *shell);
t_ast_node						*build_ast(t_token *tokens, t_shell *shell);
t_ast_node						*create_ast_node(t_node_type type, void *arena);

/* ========== EXPANDER ========== */
void							expand_variables(t_ast_node *ast,
									t_shell *shell);
char							**expand_wildcard(char *pattern,
									t_shell *shell);
char							*expand_env_var(char *str, t_shell *shell);

/* ========== UTILS ========== */
int								is_whitespace(char c);
int								is_special_char(char c);
void							free_tokens(t_token *tokens);
void							free_ast(t_ast_node *ast);
char							**split_args(char *str, void *arena);

#endif /* MINISHELL_H */
