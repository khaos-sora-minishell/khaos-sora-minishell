/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:48 by akivam            #+#    #+#             */
/*   Updated: 2025/12/12 14:22:33 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

typedef int         (*t_builtin_func)(char **args, t_shell *shell);
typedef void        (*t_transform_fn)(char *);

typedef struct s_builtin
{
    char            *name;
    t_builtin_func  func;
}                   t_builtin;

typedef struct s_echo_op
{
    char            flag;
    t_transform_fn  func;
}                   t_echo_op;

typedef struct s_echo_contex
{
    int             n;
    int             e;
    int             ops[2];
    t_gc_context    *garbage_collector_contex;
}                   t_echo_contex;

/* ========== BUILTINS ========== */
int					is_builtin(char *cmd);
int					execute_builtin(char **args, t_shell *shell);
int					builtin_echo(char **args, t_shell *shell);
int					builtin_cd(char **args, t_shell *shell);
int					builtin_pwd(char **args, t_shell *shell);
int					builtin_export(char **args, t_shell *shell);
int					builtin_unset(char **args, t_shell *shell);
int					builtin_env(char **args, t_shell *shell);
int					builtin_exit(char **args, t_shell *shell);

// echo utils 
void				op_upper(char *str);
void				op_reverse(char *str);
void				print_with_escape(char *str);
/*exstras*/
int					builtin_type(char **args, t_shell *shell);
int					builtin_tty(char **args, t_shell *shell);
int					builtin_clear(char **args, t_shell *shell);
int					builtin_true(char **args, t_shell *shell);
int					builtin_false(char **args, t_shell *shell);
int					builtin_help(char **args, t_shell *shell);

#endif
