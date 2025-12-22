/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 20:50:44 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 15:30:00 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "executor_error.h"
#include "libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include "utils.h"

void	expand_cmd_args(t_cmd *cmd, t_shell *shell)
{
	if (cmd->args)
		cmd->args = expand_args(cmd->args, shell);
}

char	*find_command_path(char *cmd, t_shell *shell)
{
	int				i;
	char			*path;
	char			*full_path;
	t_gc_context	*contex;

	if (!cmd || !shell)
		return (NULL);
	contex = (t_gc_context *)shell->cmd_arena;
	if (ft_strchr(cmd, '/'))
		return (gc_strdup(contex, cmd));
	if (!shell->path_dirs)
		return (NULL);
	i = 0;
	while (shell->path_dirs[i])
	{
		path = gc_strjoin(contex, shell->path_dirs[i], "/");
		full_path = gc_strjoin(contex, path, cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}

