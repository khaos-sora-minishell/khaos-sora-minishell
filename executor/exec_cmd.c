/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:37 by akivam            #+#    #+#             */
/*   Updated: 2025/12/14 21:53:05 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "easteregg.h"
#include "executor.h"
#include "executor_error.h"
#include "libft.h"
#include "minishell.h"
#include "utils.h"
#include <sys/stat.h>

/*
 * Find executable path for command
 * Returns command path or NULL if not found
 */
char	*find_command_path(char *cmd, t_shell *shell)
{
	int				i;
	char			*path;
	char			*full_path;
	t_gc_context	*contex;

	if (!cmd || !shell)
		return (NULL);
	contex = (t_gc_context *)shell->global_arena;
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

static void	check_file_status(char *cmd_path)
{
	struct stat	path_stat;

	if (stat(cmd_path, &path_stat) == -1)
	{
		print_execution_error(cmd_path, ERR_NO_FILE);
		exit(EXIT_CMD_NOT_FOUND);
	}
	if (S_ISDIR(path_stat.st_mode))
		handle_is_directory(cmd_path);
	if (access(cmd_path, X_OK) == -1)
		handle_permission_denied(cmd_path);
}

/*
 * Child process - setup redirections and execute command
 */
static void	exec_child_process(t_cmd *cmd, t_shell *shell)
{
	char	*cmd_path;

	setup_child_signals();
	if (setup_redirections(cmd->redirs, shell) == -1)
		exit(1);
	if (cmd->args[0] && ft_strcmp(cmd->args[0], ".") == 0)
	{
		ft_putendl_fd("minishell: .: filename argument required", 2);
		exit(2);
	}
	if (cmd->args[0] && ft_strcmp(cmd->args[0], "..") == 0)
		handle_cmd_not_found("..");
	cmd_path = find_command_path(cmd->args[0], shell);
	if (!cmd_path)
		handle_cmd_not_found(cmd->args[0]);
	check_file_status(cmd_path);
	execve(cmd_path, cmd->args, shell->env_array);
	perror("minishell: execve");
	exit(1);
}

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
	ignore_signals(); 
	waitpid(pid, &status, 0);
	setup_signals();
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
