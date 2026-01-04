/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 22:26:18 by akivam            #+#    #+#             */
/*   Updated: 2026/01/01 22:57:25 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include "executor_error.h"
#include "libft.h"
#include "minishell.h"
#include "utils.h"
#include <sys/wait.h>
#include <sys/stat.h>

void	validate_and_execute(char *cmd_path, t_cmd *cmd, t_shell *shell)
{
	struct stat	path_stat;

	if (stat(cmd_path, &path_stat) == -1)
		handle_no_such_file(cmd_path, shell);
	if (S_ISDIR(path_stat.st_mode))
		handle_is_directory(cmd_path, shell);
	if (access(cmd_path, X_OK) == -1)
		handle_permission_denied(cmd_path, shell);
	execve(cmd_path, cmd->args, shell->env_array);
	perror("minishell: execve");
	cleanup_shell(shell);
	exit(1);
}

void	exec_child_process(t_cmd *cmd, t_shell *shell)
{
	char	*cmd_path;

	setup_child_signals();
	if (setup_redirections(cmd->redirs, shell) == -1)
	{
		cleanup_shell(shell);
		exit(1);
	}
	if (!cmd->args[0])
	{
		cleanup_shell(shell);
		exit(0);
	}
	if (ft_strcmp(cmd->args[0], ".") == 0)
		exit_with_error(NULL, ".: filename argument required", 2, shell);
	if (ft_strcmp(cmd->args[0], "..") == 0)
		handle_cmd_not_found("..", shell);
	cmd_path = find_command_path(cmd->args[0], shell);
	if (!cmd_path)
		handle_cmd_not_found(cmd->args[0], shell);
	validate_and_execute(cmd_path, cmd, shell);
}

void	execute_builtin_with_redir(t_cmd *cmd, t_shell *shell)
{
	shell->redir_stdin_backup = dup(STDIN_FILENO);
	shell->redir_stdout_backup = dup(STDOUT_FILENO);
	if (setup_redirections(cmd->redirs, shell) == 0)
		shell->exit_status = execute_builtin(cmd->args, shell);
	else
		shell->exit_status = 1;
	dup2(shell->redir_stdin_backup, STDIN_FILENO);
	dup2(shell->redir_stdout_backup, STDOUT_FILENO);
	close(shell->redir_stdin_backup);
	close(shell->redir_stdout_backup);
	shell->redir_stdin_backup = -1;
	shell->redir_stdout_backup = -1;
	clean_heredoc(cmd);
}

int	prepare_cmd_execution(t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir;

	if (!cmd)
		return (0);
	if (process_cmd_heredoc(cmd, shell) == -1)
	{
		shell->exit_status = 1;
		clean_heredoc(cmd);
		return (0);
	}
	expand_cmd_args(cmd, shell);
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->file)
			redir->file = expand_string(redir->file, shell);
		redir = redir->next;
	}
	if (!cmd->args || !cmd->args[0])
	{
		clean_heredoc(cmd);
		return (0);
	}
	return (1);
}
