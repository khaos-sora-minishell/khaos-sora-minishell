/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 04:06:13 by harici            #+#    #+#             */
/*   Updated: 2025/12/22 07:33:25 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_arg(char *arg, char ***result, int *idx, t_shell *shell)
{
	char	**wildcard_res;
	char	*var_expanded;

	var_expanded = expand_string(arg, shell);
	if (!var_expanded)
		var_expanded = gc_strdup(shell->cmd_arena, arg);
	if (!has_quotes(arg))
	{
		wildcard_res = expand_wildcard(var_expanded, shell);
		if (wildcard_res)
			add_expanded_results(result, idx, wildcard_res);
		else
			(*result)[(*idx)++] = var_expanded;
	}
	else
		(*result)[(*idx)++] = var_expanded;
}

static int	count_single_expanded(char *arg, t_shell *shell)
{
	char	**wildcard_res;
	char	*var_expanded;
	int		count;

	var_expanded = expand_string(arg, shell);
	if (!var_expanded)
		var_expanded = arg;
	if (!has_quotes(arg))
	{
		wildcard_res = expand_wildcard(var_expanded, shell);
		if (wildcard_res)
			count = count_args(wildcard_res);
		else
			count = 1;
	}
	else
		count = 1;
	return (count);
}

static int	count_total_expanded(char **args, t_shell *shell)
{
	int	total;
	int	i;

	total = 0;
	i = 0;
	while (args[i])
	{
		total += count_single_expanded(args[i], shell);
		i++;
	}
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
