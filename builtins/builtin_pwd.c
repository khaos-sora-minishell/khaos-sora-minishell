/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:58 by akivam            #+#    #+#             */
/*   Updated: 2025/12/08 22:30:45 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "printf.h"
#include "garbage_collector.h"

/*
** builtin_pwd - Print Working Directory
** Prints the absolute path of the current working directory
*/
int	builtin_pwd(char **args, t_shell *shell)
{
	char			*current_path;
	t_gc_context	*gc;

	(void)args;
	gc = (t_gc_context *)shell->global_arena;
	current_path = getcwd(NULL, 0);
	if (!current_path)
	{
		perror("minishell: pwd");
		return (1);
	}
	gc_track(gc, current_path);
	printf("%s\n", current_path);
	return (0);
}
