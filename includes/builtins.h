/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:48 by akivam            #+#    #+#             */
/*   Updated: 2025/11/19 19:20:48 by akivam           ###   ########.fr       */
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

/* ========== BUILTINS ========== */
int					is_builtin(char *cmd);
int					execute_builtin(char **args, t_shell *shell);
int					builtin_echo(char **args, t_shell *shell);
int					builtin_cd(char **args, t_shell *shell);
int					builtin_pwd(char **args, t_shell *shell);
int					builtin_export(char **args, t_shell *shell);
int					builtin_unset(char **args, t_shell *shell);
int					builtin_env(t_shell *shell);
int					builtin_exit(char **args, t_shell *shell);

#endif
