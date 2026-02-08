/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_substitution.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 02:13:18 by akivam            #+#    #+#             */
/*   Updated: 2026/02/09 02:13:20 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <sys/wait.h>

static char	*extract_command_from_parens(char *str, int *i, t_shell *shell)
{
	int		start;
	int		depth;
	char	*command;

	(*i)++;
	start = *i;
	depth = 1;
	while (str[*i] && depth > 0)
	{
		if (str[*i] == '(')
			depth++;
		else if (str[*i] == ')')
			depth--;
		if (depth > 0)
			(*i)++;
	}
	if (depth != 0)
		return (gc_strdup(shell->cmd_arena, ""));
	command = gc_strndup(shell->cmd_arena, str + start, *i - start);
	if (str[*i] == ')')
		(*i)++;
	return (command);
}

static char	*read_pipe_output(int fd, t_shell *shell)
{
	char	buffer[4096];
	char	*result;
	char	*temp;
	ssize_t	bytes;

	result = gc_strdup(shell->cmd_arena, "");
	while (1)
	{
		bytes = read(fd, buffer, sizeof(buffer) - 1);
		if (bytes <= 0)
			break ;
		buffer[bytes] = '\0';
		temp = result;
		result = gc_strjoin(shell->cmd_arena, temp, buffer);
	}
	return (result);
}

static void	child_execute_command(char *command, t_shell *shell, int write_fd)
{
	close(write_fd);
	shell->ast_root = parser(lexer(command, shell), shell);
	if (shell->ast_root)
		executor_run(shell);
	exit(shell->exit_status);
}

static char	*parent_read_output(int read_fd, t_shell *shell)
{
	char	*output;
	int		status;

	output = read_pipe_output(read_fd, shell);
	close(read_fd);
	waitpid(-1, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	return (output);
}

static char	*execute_command_in_subshell(char *command, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (gc_strdup(shell->cmd_arena, ""));
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (gc_strdup(shell->cmd_arena, ""));
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		child_execute_command(command, shell, pipefd[1]);
	}
	close(pipefd[1]);
	return (parent_read_output(pipefd[0], shell));
}

static char	*trim_trailing_newline(char *str, t_shell *shell)
{
	int	len;

	if (!str)
		return (gc_strdup(shell->cmd_arena, ""));
	len = ft_strlen(str);
	while (len > 0 && str[len - 1] == '\n')
		len--;
	return (gc_strndup(shell->cmd_arena, str, len));
}

char	*process_command_substitution(char *str, int *i, t_shell *shell)
{
	char	*command;
	char	*output;

	command = extract_command_from_parens(str, i, shell);
	if (!command || !*command)
		return (gc_strdup(shell->cmd_arena, ""));
	output = execute_command_in_subshell(command, shell);
	output = trim_trailing_newline(output, shell);
	return (output);
}
