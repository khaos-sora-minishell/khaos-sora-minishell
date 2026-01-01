/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:24:21 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 13:44:01 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "utils.h"

int	heredoc_signal_checker(void)
{
	if (get_signal() == SIGINT)
		rl_done = 1;
	return (0);
}

int	process_heredoc_line(char *line, char *delim, int fd)
{
	if (get_signal() == SIGINT)
	{
		if (line)
			free(line);
		return (1);
	}
	if (!line)
		return (1);
	if (ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (1);
	}
	ft_putendl_fd(line, fd);
	free(line);
	return (0);
}
