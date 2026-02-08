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
}

static void	handle_no_command(t_cmd *cmd, t_shell *shell)
{
	if (cmd && cmd->redirs)
		handle_redirection_only(cmd, shell);
	else if (cmd && cmd->args && cmd->args[0] && !cmd->args[0][0])
	{
		print_execution_error("", ERR_NO_FILE);
		shell->exit_status = EXIT_CMD_NOT_FOUND;
	}
}

void	execute_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!prepare_cmd_execution(cmd, shell))
		return (handle_no_command(cmd, shell));
	if (is_builtin(cmd->args[0]))
		return (execute_builtin_with_redir(cmd, shell));
	pid = fork();
	if (pid == -1)
		return (perror("fork"), (void)(shell->exit_status = 1));
	if (pid == 0)
		exec_child_process(cmd, shell);
	ignore_signals();
	waitpid(pid, &status, 0);
	setup_signals();
	handle_exit_status(shell, status);
}
