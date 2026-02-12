/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 22:27:10 by akivam            #+#    #+#             */
/*   Updated: 2026/01/01 22:27:10 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "executor_error.h"
#include "libft.h"
#include "shell.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include "utils.h"

void	expand_cmd_args(t_cmd *cmd, t_shell *shell)
{
	if (cmd->args)
		cmd->args = expand_args(cmd->args, shell);
}

void	handle_exit_status(t_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", 2);
	}
}

static char	*search_in_path_dirs(char *cmd, t_shell *shell, t_gc_context *ctx)
{
	int		i;
	char	*path;
	char	*full_path;

	i = 0;
	while (shell->path_dirs[i])
	{
		path = gc_strjoin(ctx, shell->path_dirs[i], "/");
		full_path = gc_strjoin(ctx, path, cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}

char	*find_command_path(char *cmd, t_shell *shell)
{
	char			*full_path;
	t_gc_context	*contex;

	if (!cmd || !shell)
		return (NULL);
	contex = (t_gc_context *)shell->cmd_arena;
	if (ft_strchr(cmd, '/'))
		return (gc_strdup(contex, cmd));
	if (!shell->path_dirs)
	{
		full_path = gc_strjoin(contex, "./", cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		return (NULL);
	}
	return (search_in_path_dirs(cmd, shell, contex));
}
