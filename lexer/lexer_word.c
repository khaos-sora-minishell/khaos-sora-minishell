/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:30:46 by harici            #+#    #+#             */
/*   Updated: 2026/01/13 20:30:46 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

void	skip_quoted(char *input, int *i)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
		(*i)++;
}

void	find_word_end(char *input, int *i)
{
	while (input[*i] && !is_whitespace(input[*i])
		&& !is_special_char(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '\"')
			skip_quoted(input, i);
		else
			(*i)++;
	}
}

char	*extract_word(char *input, int *i, t_shell *shell)
{
	int		start;
	int		len;
	char	*word;
	int		j;

	start = *i;
	find_word_end(input, i);
	len = *i - start;
	word = gc_malloc(shell->cmd_arena, len + 1);
	if (!word)
		return (NULL);
	j = 0;
	while (j < len)
	{
		word[j] = input[start + j];
		j++;
	}
	word[j] = '\0';
	return (word);
}
