/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 01:43:20 by akivam            #+#    #+#             */
/*   Updated: 2026/02/12 01:43:26 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <readline/readline.h>
#include <unistd.h>

char	*remove_trailing_newline(char *str)
{
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len > 0 && str[len - 1] == '\n')
		str[len - 1] = '\0';
	return (str);
}

char	*get_continuation_line(char quote)
{
	char	*next_line;
	char	*prompt;

	prompt = "dquote> ";
	if (quote == '\'')
		prompt = "squote> ";
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		next_line = readline(prompt);
	else
	{
		next_line = get_next_line(STDIN_FILENO);
		next_line = remove_trailing_newline(next_line);
	}
	return (next_line);
}
