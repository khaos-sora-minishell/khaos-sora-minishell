/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:48 by akivam            #+#    #+#             */
/*   Updated: 2026/02/06 22:49:08 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

typedef int			(*t_builtin_func)(char **args, t_shell *shell);

typedef struct s_builtin
{
	char			*name;
	t_builtin_func	func;
}					t_builtin;

typedef struct s_echo_contex
{
	int				n;
	t_gc_context	*garbage_collector_contex;
}					t_echo_contex;

/* ========== BUILTINS ========== */
int					is_builtin(char *cmd);
int					execute_builtin(char **args, t_shell *shell);
int					builtin_echo(char **args, t_shell *shell);
int					builtin_cd(char **args, t_shell *shell);
int					builtin_pwd(char **args, t_shell *shell);

int					builtin_export(char **args, t_shell *shell);
void				sort_buckets(t_env_bucket **arr, int count);
void				print_export_item(t_env_bucket *item, t_shell *shell);
void				print_sorted_env(t_shell *shell);

int					builtin_unset(char **args, t_shell *shell);
int					builtin_env(char **args, t_shell *shell);
int					builtin_exit(char **args, t_shell *shell);

# ifdef BONUS

/* ========== BONUS BUILTINS ========== */
int					builtin_alias(char **args, t_shell *shell);
int					builtin_unalias(char **args, t_shell *shell);
int					builtin_help(char **args, t_shell *shell);
int					builtin_tty(char **args, t_shell *shell);
int					builtin_type(char **args, t_shell *shell);
int					builtin_true(char **args, t_shell *shell);
int					builtin_false(char **args, t_shell *shell);

# endif

#endif
