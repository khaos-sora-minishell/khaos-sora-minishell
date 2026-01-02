/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:57 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 18:47:42 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include "libft.h"
#include "minishell.h"
#include "utils.h"
#include <errno.h>

static void	update_pwd_env(t_shell *shell, char *old_path)
{
	char			*current_path;
	t_gc_context	*contex;

	contex = (t_gc_context *)shell->global_arena;
	current_path = getcwd(NULL, 0);
	if (!current_path)
	{
		perror("minishell: cd: error retrieving current directory");
		return ;
	}
	gc_track(contex, current_path);	
	if (old_path)
		env_set(shell->env_table, "OLDPWD", old_path, contex);	
	env_set(shell->env_table, "PWD", current_path, contex);
	shell->env_array = env_table_to_array(shell->env_table, contex);
}

static char	*resolve_path(char **args, t_shell *shell)
{
	char			*path;
	t_gc_context	*contex;

	contex = (t_gc_context *)shell->global_arena;
	if (!args[1])
	{
		path = env_get(shell->env_table, "HOME", contex);
		if (!path)
		{
			ft_err_printf("minishell: cd: HOME not set\n");
			return (NULL);
		}
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		path = env_get(shell->env_table, "OLDPWD", contex);
		if (!path)
			return (ft_putendl_fd("minishell: cd: OLDPWD not set", 2), NULL);
		ft_printf("%s\n", path);
	}
	else
		path = args[1];
	return (path);
}

int	builtin_cd(char **args, t_shell *shell)
{
	char			*target_path;
	char			*old_path;
	t_gc_context	*contex;

	contex = (t_gc_context *)shell->global_arena;
	old_path = env_get(shell->env_table, "PWD", contex);
	if (!old_path)
	{
		old_path = getcwd(NULL, 0);
		if (old_path)
			gc_track(contex, old_path);
	}
	if (args[1] && args[2])
		return (ft_putendl_fd("minishell: cd: too many arguments", 2), 1);
	target_path = resolve_path(args, shell);
	if (!target_path)
		return (1);
	if (chdir(target_path) == -1)
	{
		ft_err_printf("minishell: cd: %s: %s\n", args[1], strerror(errno));
		return (1);
	}
	update_pwd_env(shell, old_path);
	return (0);
}
