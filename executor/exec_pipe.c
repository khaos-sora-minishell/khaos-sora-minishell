/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:38 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 14:26:22 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	exec_left_pipe(t_ast_node *left, t_shell *shell, int *pipefd)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	execute_ast(left, shell);
	cleanup_child_process(shell);
	exit(shell->exit_status);
}

static void	exec_right_pipe(t_ast_node *right, t_shell *shell, int *pipefd)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	execute_ast(right, shell);
	cleanup_child_process(shell);
	exit(shell->exit_status);
}

void	execute_pipe(t_ast_node *left, t_ast_node *right, t_shell *shell)
{
	pid_t	left_pid;
	pid_t	right_pid;
	int		status;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		shell->exit_status = 1;
		return ;
	}
	left_pid = fork();
	if (left_pid == 0)
		exec_left_pipe(left, shell, pipefd);
	right_pid = fork();
	if (right_pid == 0)
		exec_right_pipe(right, shell, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
}
