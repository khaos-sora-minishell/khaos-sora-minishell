/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:37 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 14:32:06 by akivam           ###   ########.fr       */
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

static void	validate_and_execute(char *cmd_path, t_cmd *cmd, t_shell *shell)
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

static void	exec_child_process(t_cmd *cmd, t_shell *shell)
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

static void	execute_builtin_with_redir(t_cmd *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (setup_redirections(cmd->redirs, shell) == 0)
		shell->exit_status = execute_builtin(cmd->args, shell);
	else
		shell->exit_status = 1;
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	clean_heredoc(cmd);
}

static void	handle_exit_status(t_shell *shell, int status)
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

/* YENİ YERİ: Tam execute_command'ın üstünde */
static int	prepare_cmd_execution(t_cmd *cmd, t_shell *shell)
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

static void	handle_redirection_only(t_cmd *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (setup_redirections(cmd->redirs, shell) == -1)
		shell->exit_status = 1;
	else
		shell->exit_status = 0;
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	clean_heredoc(cmd);
}

void	execute_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!prepare_cmd_execution(cmd, shell))
	{
		if (cmd && cmd->redirs)
			handle_redirection_only(cmd, shell);
		return ;
	}
	if (is_builtin(cmd->args[0]))
		return (execute_builtin_with_redir(cmd, shell));
	pid = fork();
	if (pid == -1)
		return (perror("fork"), (void)(shell->exit_status = 1),
			clean_heredoc(cmd));
	if (pid == 0)
		exec_child_process(cmd, shell);
	ignore_signals();
	waitpid(pid, &status, 0);
	setup_signals();
	handle_exit_status(shell, status);
	clean_heredoc(cmd);
}
