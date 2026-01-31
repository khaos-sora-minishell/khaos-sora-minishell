/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:58 by akivam            #+#    #+#             */
/*   Updated: 2025/12/15 21:22:41 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include "minishell.h"

/*
** builtin_pwd - Print Working Directory
** Prints the absolute path of the current working directory
*/
int	builtin_pwd(char **args, t_shell *shell)
{
	char			*current_path;
	t_gc_context	*contex;

	(void)args;
	contex = (t_gc_context *)shell->cmd_arena;
	current_path = env_get(shell->env_table, "PWD", contex);
	if (!current_path)
	{
		current_path = getcwd(NULL, 0);
		if (!current_path)
		{
			perror("minishell: pwd");
			return (1);
		}
		gc_track(contex, current_path);
	}
	ft_printf("%s\n", current_path);
	return (0);
}
