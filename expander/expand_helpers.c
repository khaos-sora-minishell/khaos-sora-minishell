/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 07:30:00 by harici            #+#    #+#             */
/*   Updated: 2025/12/22 10:02:01 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(char **args)
{
	int	count;

	count = 0;
	if (!args)
		return (0);
	while (args[count])
		count++;
	return (count);
}

int	has_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

int	add_expanded_results(char ***result, int *idx, char **wildcard_result)
{
	int	j;

	j = 0;
	while (wildcard_result[j])
	{
		(*result)[*idx] = wildcard_result[j];
		(*idx)++;
		j++;
	}
	return (j);
}
