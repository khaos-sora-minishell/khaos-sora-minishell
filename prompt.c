/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 21:13:02 by harici            #+#    #+#             */
/*   Updated: 2026/01/05 21:13:02 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "libft.h"
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

static char	*read_continuation(char *input, char quote, t_shell *shell)
{
	char	*next_line;
	char	*result;

	next_line = get_continuation_line(quote);
	if (!next_line)
	{
		ft_putstr_fd("minishell: unexpected EOF while looking for "
			"matching `", 2);
		ft_putchar_fd(quote, 2);
		ft_putendl_fd("'", 2);
		shell->exit_status = 2;
		free(input);
		return (NULL);
	}
	result = join_lines(input, next_line);
	free(input);
	free(next_line);
	return (result);
}

char	*read_multiline(t_shell *shell)
{
	char	*input;
	char	quote;

	if (isatty(STDIN_FILENO))
		input = readline(get_prompt(shell));
	else
	{
		input = get_next_line(STDIN_FILENO);
		input = remove_trailing_newline(input);
	}
	if (!input)
		return (NULL);
	quote = check_unclosed_quotes_char(input);
	while (quote)
	{
		input = read_continuation(input, quote, shell);
		if (!input)
			return (NULL);
		quote = check_unclosed_quotes_char(input);
	}
	return (input);
}
