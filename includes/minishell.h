/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:48 by akivam            #+#    #+#             */
/*   Updated: 2025/11/19 19:20:48 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                  minishell.h                               */
/*                      -- PROJE ANAYASASI TASLAĞI v2.0 --                     */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ========================================================================== */
/*                               INCLUDES                                     */
/* ========================================================================== */

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "garbage_collector.h"

/* ========================================================================== */
/*                          GLOBAL SIGNAL VARIABLE                            */
/* ========================================================================== */

/*
** TEK İZİN VERİLEN GLOBAL DEĞİŞKEN (Subject şartı)
** - SADECE signal numarasını tutar
** - sig_atomic_t: Signal handler'da güvenli erişim için
** - volatile: Compiler optimization'ı engeller
** - Başka hiçbir global değişken kullanılmamalı!
*/
extern volatile sig_atomic_t	g_signal;

/* ========================================================================== */
/*                            ENUMERATIONS                                    */
/* ========================================================================== */

/*
** Lexer'ın üreteceği token tipleri
** Her metakarakter ve kelime tipi burada tanımlanır
*/
typedef enum e_token_type
{
	TOKEN_WORD,         // Normal kelime, argüman, dosya adı
	TOKEN_PIPE,         // |  (pipe)
	TOKEN_REDIR_IN,     // <  (input redirection)
	TOKEN_REDIR_OUT,    // >  (output redirection)
	TOKEN_REDIR_APPEND, // >> (append mode output)
	TOKEN_HEREDOC,      // << (here document)
	/* --- BONUS TOKENS --- */
	TOKEN_AND,    // && (logical AND)
	TOKEN_OR,     // || (logical OR)
	TOKEN_LPAREN, // (  (left parenthesis)
	TOKEN_RPAREN, // )  (right parenthesis)
}								t_token_type;

/*
** AST düğüm tipleri
** Her düğüm, komutlar arasındaki ilişkiyi tanımlar
*/
typedef enum e_node_type
{
	NODE_CMD,  // Yaprak düğüm: Tek bir komut
	NODE_PIPE, // Dal düğüm: Pipe ile bağlı iki komut
	/* --- BONUS NODE TYPES --- */
	NODE_AND,     // Dal düğüm: && operatörü
	NODE_OR,      // Dal düğüm: || operatörü
	NODE_SUBSHELL // Dal düğüm: () ile gruplanmış komutlar
}								t_node_type;

/* ========================================================================== */
/*                           DATA STRUCTURES                                  */
/* ========================================================================== */

/*
** Token yapısı (Lexer'ın çıktısı)
** Lexer, input string'i bu yapılardan oluşan linked list'e çevirir
*/
typedef struct s_token
{
	t_token_type type;    // Token'ın tipi (WORD, PIPE, vb.)
	char *value;          // Token'ın string değeri
	struct s_token *next; // Sonraki token (linked list)
}								t_token;

/*
** Yönlendirme yapısı
** Her komutun yönlendirme bilgilerini tutar (<, >, >>, <<)
*/
typedef struct s_redir
{
	t_token_type type;     // Yönlendirme tipi
	char *file;            // Hedef dosya adı
	char *delimiter;       // SADECE heredoc için (<<)
	char *heredoc_tmpfile; // Heredoc için temp dosya yolu
	struct s_redir *next;  // Sonraki yönlendirme
}								t_redir;

/*
** Basit komut yapısı
** Tek bir komutu (argümanları ve yönlendirmeleriyle) temsil eder
** Parser tarafından doldurulur, Executor tarafından çalıştırılır
*/
typedef struct s_cmd
{
	char **args;     // execve'ye uygun argüman dizisi (NULL-terminated)
	t_redir *redirs; // Bu komuta ait yönlendirmeler (linked list)
}								t_cmd;

/*
** Ortam değişkeni yapısı
** Shell'in environment variable'larını tutar (export, unset için)
*/
typedef struct s_env
{
	char *key;          // Değişken ismi (örn: "PATH")
	char *value;        // Değişken değeri
	struct s_env *next; // Sonraki env değişkeni
}								t_env;

/*
** AST (Abstract Syntax Tree) düğüm yapısı
** Komutların hiyerarşik yapısını recursive olarak temsil eder
*/
typedef struct s_ast_node
{
	t_node_type type; // Düğüm tipi (CMD, PIPE, AND, OR, SUBSHELL)

	/* Operatör düğümleri için (PIPE, AND, OR) */
	struct s_ast_node *left;  // Sol alt ağaç
	struct s_ast_node *right; // Sağ alt ağaç

	/* Subshell düğümleri için (SUBSHELL) */
	struct s_ast_node *subshell_node; // İçerideki ağaç

	/* Komut düğümleri için (CMD) */
	t_cmd *cmd; // Komut bilgileri
}								t_ast_node;

/*
** Ana shell yapısı
** Shell'in tüm durumunu ve global bilgilerini tutar
** Fonksiyonlara pointer olarak geçirilir (global değişken yerine)
*/
typedef struct s_shell
{
	/* Garbage Collector Arenaları */
	void *global_arena; // Shell lifetime boyunca kalır
	void *cmd_arena;    // Her komut için yeniden oluşturulur

	/* Ortam Değişkenleri */
	t_env *env_list;  // Linked list formatında
	char **env_array; // execve için char** formatında

	/* PATH Yönetimi */
	char **path_dirs; // PATH'ten split edilmiş dizinler

	/* Shell Durumu */
	int exit_status;      // Son komutun çıkış kodu ($? için)
	t_ast_node *ast_root; // Parser'ın oluşturduğu AST'nin kökü

	/* File Descriptor Yedekleri */
	int stdin_backup;  // stdin'i restore etmek için
	int stdout_backup; // stdout'u restore etmek için

	/* History Yönetimi (Opsiyonel) */
	// char		*history_file;  // ~/.minishell_history
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
