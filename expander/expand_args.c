/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:27:27 by harici            #+#    #+#             */
/*   Updated: 2026/01/13 20:27:27 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	process_words(char **words, char ***result, int *idx,
		t_shell *shell)
{
	int	i;

	i = 0;
	while (words[i])
		add_word_or_wc(words[i++], result, idx, shell);
}

static void	process_arg(char *arg, char ***result, int *idx, t_shell *shell)
{
	char	*exp;
	char	**words;

	exp = expand_string(arg, shell);
	if (!exp)
		exp = gc_strdup(shell->cmd_arena, arg);
	if (!has_quotes(arg))
	{
		words = split_words(exp, shell);
		if (words)
			process_words(words, result, idx, shell);
		else if (exp[0] != '\0')
			add_word_or_wc(exp, result, idx, shell);
	}
	else
		(*result)[(*idx)++] = exp;
}

static int	count_single_expanded(char *arg, t_shell *shell)
{
	char	*exp;
	char	**words;
	int		count;
	int		i;

	exp = expand_string(arg, shell);
	if (!exp)
		exp = arg;
	if (has_quotes(arg))
		return (1);
	words = split_words(exp, shell);
	if (!words)
		return (exp[0] != '\0');
	count = 0;
	i = 0;
	while (words[i])
		count += count_word_with_wildcard(words[i++], shell);
	return (count);
}

static int	count_total_expanded(char **args, t_shell *shell)
{
	int	total;
	int	i;

	total = 0;
	i = 0;
	while (args[i])
		total += count_single_expanded(args[i++], shell);
	return (total);
}

char	**expand_args(char **args, t_shell *shell)
{
	char	**result;
	int		total;
	int		idx;
	int		i;

	if (!args || !shell)
		return (args);
	total = count_total_expanded(args, shell);
	result = gc_malloc(shell->cmd_arena, sizeof(char *) * (total + 1));
	if (!result)
		return (args);
	idx = 0;
	i = 0;
	while (args[i])
		process_arg(args[i++], &result, &idx, shell);
	result[idx] = NULL;
	return (result);
}
