/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:58 by akivam            #+#    #+#             */
/*   Updated: 2025/12/15 20:43:47 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include "minishell.h"
#include "printf.h"

/*
** builtin_pwd - Print Working Directory
** Prints the absolute path of the current working directory
*/
int	builtin_pwd(char **args, t_shell *shell)
{
	char			*current_path;
	t_gc_context	*contex;

	(void)args;
	contex = (t_gc_context *)shell->global_arena;
	current_path = getcwd(NULL, 0);
	if (!current_path)
	{
		current_path = env_get(shell->env_list, "PWD");
		if (!current_path)
		{
			perror("minishell: pwd");
			return (1);
		}
	}
	else
		gc_track(contex, current_path);
	printf("%s\n", current_path);
	return (0);
}
