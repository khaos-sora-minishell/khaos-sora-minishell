/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 20:50:44 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 22:18:32 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "executor_error.h"
#include "libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdio.h>
#include <sys/stat.h>
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

static void	read_heredoc_loop(int fd, char *delim, t_shell *shell)
{
	char	*line;
	char	*prompt;

	prompt = env_get(shell->env_table, "PS2", shell->cmd_arena);
	if (!prompt)
		prompt = "> ";
	while (1)
	{
		line = readline(prompt);
		if (!line)
			break ;
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

int	process_cmd_heredoc(t_cmd *cmd, t_shell *shell)
{
	t_redir	*current;
	char	*tmp_file;
	int		counter;
	int		fd;

	current = cmd->redirs;
	counter = 0;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			tmp_file = gc_strjoin(shell->cmd_arena, "/tmp/.heredoc_",
					gc_itoa(shell->cmd_arena, counter));
			fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				return (perror("heredoc open"), -1);
			read_heredoc_loop(fd, current->delimiter, shell);
			close(fd);
			current->type = TOKEN_REDIR_IN;
			current->file = tmp_file;
		}
		current = current->next;
		counter++;
	}
	return (0);
}

void	check_file_status(char *cmd_path)
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
