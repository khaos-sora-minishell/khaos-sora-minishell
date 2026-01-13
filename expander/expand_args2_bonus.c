/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args2_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:27:36 by harici            #+#    #+#             */
/*   Updated: 2026/01/13 20:27:36 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_words(char *str)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && is_ifs(str[i]))
			i++;
		if (str[i])
		{
			count++;
			while (str[i] && !is_ifs(str[i]))
				i++;
		}
	}
	return (count);
}

char	**split_words(char *str, t_shell *shell)
{
	char	**words;
	int		wc;
	int		i;
	int		w;
	int		start;

	wc = count_words(str);
	if (wc == 0)
		return (NULL);
	words = gc_malloc(shell->cmd_arena, sizeof(char *) * (wc + 1));
	i = 0;
	w = 0;
	while (str[i] && w < wc)
	{
		while (str[i] && is_ifs(str[i]))
			i++;
		start = i;
		while (str[i] && !is_ifs(str[i]))
			i++;
		if (i > start)
			words[w++] = gc_substr(shell->cmd_arena, str, start, i - start);
	}
	words[w] = NULL;
	return (words);
}

int	count_word_with_wildcard(char *word, t_shell *shell)
{
	char	**wildcard_res;

	wildcard_res = expand_wildcard(word, shell);
	if (wildcard_res)
		return (count_args(wildcard_res));
	else if (word[0] != '\0')
		return (1);
	return (0);
}

void	add_word_or_wc(char *word, char ***result, int *idx, t_shell *shell)
{
	char	**wc;

	wc = expand_wildcard(word, shell);
	if (wc)
		add_expanded_results(result, idx, wc);
	else if (word[0] != '\0')
		(*result)[(*idx)++] = word;
}
