/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:48 by akivam            #+#    #+#             */
/*   Updated: 2025/12/18 21:48:34 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* */
/* minishell.h                               */
/* -- PROJE ANAYASASI TASLAĞI v2.2 --                     */
/* */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ========================================================================== */
/* INCLUDES                                     */
/* ========================================================================== */

# include "garbage_collector.h"
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

/* --- BASH SETTINGS & MACROS --- */
# define DEFAULT_PATH_VALUE "/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin:/bin:/sbin:."

/* --- HASH TABLE & CRYPTO SETTINGS --- */
# define ENV_TABLE_SIZE 131             // Asal sayı
# define FNV_PRIME_64 1099511628211UL
# define FNV_OFFSET 14695981039346656037UL
# define XOR_KEY 0xFF                   // Şifreleme anahtarı

/* ========================================================================== */
/* GLOBAL SIGNAL VARIABLE                            */
/* ========================================================================== */
extern volatile sig_atomic_t	g_signal;

/* ========================================================================== */
/* ENUMERATIONS                                    */
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
}	t_token_type;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL,
}	t_node_type;

/* ========================================================================== */
/* DATA STRUCTURES                                  */
/* ========================================================================== */

/* --- HASH TABLE YAPILARI (YENİ) --- */
typedef struct s_env_bucket
{
    char                    *key;
    char                    *value;     // XOR ile şifrelenmiş veri
	int 					_has_value;
    struct s_env_bucket     *next;      // Collision zinciri
}   t_env_bucket;

typedef struct s_env_table
{
    t_env_bucket    **buckets;          // Bucket dizisi
    int             count;              // Toplam eleman sayısı
}   t_env_table;

/* --- MEVCUT YAPILAR --- */
typedef struct s_token
{
	t_token_type    type;
	char            *value;
	struct s_token  *next;
}	t_token;

typedef struct s_redir
{
	t_token_type    type;
	char            *file;
	char            *delimiter;
	char            *heredoc_tmpfile;
	struct s_redir  *next;
}	t_redir;

typedef struct s_cmd
{
	char        **args;
	t_redir     *redirs;
}	t_cmd;

/* NOT: Bu eski t_env yapısı export sıralaması vb. için geçici olarak kalabilir,
   ancak ana yapı t_env_table olacak. */
typedef struct s_env
{
	char            *key;
	char            *value;
	struct s_env    *next;
}	t_env;

typedef struct s_ast_node
{
	t_node_type         type;
	struct s_ast_node   *left;
	struct s_ast_node   *right;
	struct s_ast_node   *subshell_node;
	t_cmd               *cmd;
}	t_ast_node;

/* --- GÜNCELLENMİŞ SHELL YAPISI --- */
typedef struct s_shell
{
    /* Garbage Collector Arenaları (EKSİKTİ, EKLENDİ) */
    void            *global_arena;  // Shell lifetime boyunca kalır
    void            *cmd_arena;     // Her komut için temizlenir

    /* Terminal Ayarları */
    char			*terminal_name;
	char			*terminal_name_colo;
	char			*terminal_text_color;
	char			*terminal_bg_color;

    /* Ortam Değişkenleri */
    t_env_table     *env_table;     // YENİ: Hash Table (env_list YERİNE)
	char            **env_array;    // execve için char** formatında
	t_env           *alias_list;    // alias için linked list

	/* PATH Yönetimi */
	char            **path_dirs;    // PATH dizinleri

	/* Shell Durumu */
	int             exit_status;    // $? değeri
	t_ast_node      *ast_root;      // Parser ağacı

	/* Yedekler */
	int             stdin_backup;
	int             stdout_backup;
	char            *history_file;
}   t_shell;


/* ========================================================================== */
/* FUNCTION PROTOTYPES                                 */
/* ========================================================================== */

/* --- ENV MANAGER (env_manager.c) - SENİN İSTEDİĞİN İSİMLERLE --- */
t_env_table     *initilaze_env_table(char **envp, void *arena);
char            *env_get(t_env_table *table, char *key, void *arena);
void            env_set(t_env_table *table, char *key, char *value, void *arena);
void            env_unset(t_env_table *table, char *key);
char            **env_table_to_array(t_env_table *table, void *arena);

/* --- CRYPTO (env_crypto.c) --- */
unsigned long   fnv1a_hash(char *str);
void            xor_cipher(char *str);

/* ========== MAIN ========== */
#ifndef TEST_MODE
int				main(int ac, char **av, char **envp);
#endif
void			init_shell(t_shell *shell, char **envp);
void			cleanup_shell(t_shell *shell);
char            **parse_path(t_shell *shell); // Argüman güncellendi

/* ========== ENVIRONMENT (ESKİLER SİLİNDİ) ========== */
/* init_env, set_env_value vb. buradaydı, sildik çünkü çakışma yapıyordu */

/* ========== PATH HANDLING ========== */
char			*find_command_path(char *cmd, t_shell *shell);

/* ========== SIGNALS ========== */
void			setup_signals(void);
void			signal_handler(int signum);
void			setup_child_signals(void);
void			ignore_signals(void);

/* ========== ERROR HANDLING ========== */
void			error_exit(char *msg, t_shell *shell);
void			print_error(const char *cmd, const char *arg, const char *msg);
void			syntax_error(char *token);
int				command_not_found(char *cmd);

/* ========== LEXER & PARSER ========== */
t_token			*lexer(char *input, t_shell *shell);
t_ast_node		*parser(t_token *tokens, t_shell *shell);
t_token			*create_token(t_token_type type, char *value, void *arena);
void			add_token(t_token **list, t_token *new_token);
int             is_whitespace(char c);
int             is_special_char(char c);

/* ========== EXECUTOR ========== */
void            executor_run(t_shell *shell); 

/* ========== UTILS ========== */
void			free_tokens(t_token *tokens);
void			free_ast(t_ast_node *ast);
char			**split_args(char *str, void *arena);

#endif /* MINISHELL_H */