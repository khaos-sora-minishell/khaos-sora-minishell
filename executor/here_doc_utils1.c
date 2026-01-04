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

static size_t	calculate_expanded_length(char *line, t_shell *shell)
{
	size_t	len;
	int		i;
	char	*expanded;

	len = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			expanded = process_dollar(line, &i, shell);
			len += gc_strlen(expanded);
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

char	*expand_heredoc_line(char *line, t_shell *shell)
{
	char	*result;
	int		i;
	int		j;
	char	*expanded;
	int		exp_len;

	result = gc_malloc(shell->cmd_arena,
			calculate_expanded_length(line, shell) + 1);
	if (!result)
		return (line);
	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			expanded = process_dollar(line, &i, shell);
			exp_len = gc_strlen(expanded);
			ft_memcpy(result + j, expanded, exp_len);
			j += exp_len;
		}
		else
			result[j++] = line[i++];
	}
	result[j] = '\0';
	return (result);
}

int	process_heredoc_line(char *line, int fd, t_redir *redir, t_shell *shell)
{
	char	*expanded;

	if (get_signal() == SIGINT)
	{
		if (line)
			free(line);
		return (1);
	}
	if (!line)
		return (1);
	if (ft_strcmp(line, redir->delimiter) == 0)
	{
		free(line);
		return (1);
	}
	if (redir->should_expand)
	{
		expanded = expand_heredoc_line(line, shell);
		ft_putendl_fd(expanded, fd);
	}
	else
		ft_putendl_fd(line, fd);
	free(line);
	return (0);
}
