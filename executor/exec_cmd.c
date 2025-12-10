/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:37 by akivam            #+#    #+#             */
/*   Updated: 2025/12/08 21:14:53 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include "easteregg.h"

/*
 * Find executable path for command
 * Returns command path or NULL if not found
 */
char	*find_command_path(char *cmd, t_shell *shell)
{
	int				i;
	char			*path;
	char			*full_path;
	t_gc_context	*gc;

	if (!cmd || !shell || !shell->path_dirs)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (cmd);
		return (NULL);
	}
	gc = (t_gc_context *)shell->global_arena;
	i = 0;
	while (shell->path_dirs[i])
	{
		path = gc_strjoin(gc, shell->path_dirs[i], "/");
		full_path = gc_strjoin(gc, path, cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}

/*
 * Child process - setup redirections and execute command
 */
static void	exec_child_process(t_cmd *cmd, t_shell *shell)
{
	char	*cmd_path;

	if (setup_redirections(cmd->redirs, shell) == -1)
		exit(1);
	cmd_path = find_command_path(cmd->args[0], shell);
	if (!cmd_path)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	execve(cmd_path, cmd->args, shell->env_array);
	perror("execve");
	exit(126);
}

/*
 * Execute a single command - fork, execve
 */
void	execute_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (is_easter_egg(cmd->args[0]))
	{
		execute_easter_egg(cmd->args, shell);
		return ;
	}
	if (is_builtin(cmd->args[0]))
	{
		shell->exit_status = execute_builtin(cmd->args, shell);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		shell->exit_status = 1;
		return ;
	}
	if (pid == 0)
		exec_child_process(cmd, shell);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
}
