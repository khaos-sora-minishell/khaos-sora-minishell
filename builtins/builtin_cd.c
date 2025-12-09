/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:57 by akivam            #+#    #+#             */
/*   Updated: 2025/12/09 19:43:12 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include "libft.h"
#include "minishell.h"
#include "printf.h"
#include <errno.h>
#include "utils.h"

static void	update_pwd_env(t_shell *shell, char *old_path)
{
	char			*current_path;
	t_gc_context	*contex;

	contex = (t_gc_context *)shell->global_arena;
	current_path = getcwd(NULL, 0);
	if (!current_path)
	{
		perror("minishell: cd: getcwd");
		return ;
	}
	gc_track(contex, current_path);
	if (old_path)
		set_env_value(&shell->env_list, "OLDPWD", old_path, contex);
	set_env_value(&shell->env_list, "PWD", current_path, contex);
}

static char	*resolve_path(char **args, t_shell *shell)
{
	char	*path;

	if (!args[1])
	{
		path = get_env_value(shell->env_list, "HOME");
		if (!path)
		{
			ft_err_printf("minishell: cd: HOME not set\n");
			return (NULL);
		}
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		path = get_env_value(shell->env_list, "OLDPWD");
		if (!path)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
			return (NULL);
		}
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
	old_path = getcwd(NULL, 0);
	if (old_path)
		gc_track(contex, old_path);
	else
	{
		perror("minishell: cd: getcwd");
		return (1);
	}
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
