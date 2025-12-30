/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 00:00:05 by harici            #+#    #+#             */
/*   Updated: 2025/12/12 00:00:07 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quotes(char *str, int *i)
{
	char	quote;

	if (!str || !i)
		return (-1);
	if (str[*i] != '\'' && str[*i] != '\"')
		return (0);
	quote = str[*i];
	(*i)++;
	while (str[*i] && str[*i] != quote)
		(*i)++;
	if (str[*i] != quote)
		return (-1);
	(*i)++;
	return (0);
}

static void	copy_quoted_content(char *str, char *result, int *i, int *j)
{
	char	quote;

	quote = str[*i];
	(*i)++;
	while (str[*i] && str[*i] != quote)
	{
		result[*j] = str[*i];
		(*j)++;
		(*i)++;
	}
	if (str[*i] == quote)
		(*i)++;
}

char	*remove_quotes(char *str, void *arena)
{
	int		i;
	int		j;
	char	*result;

	if (!str)
		return (NULL);
	result = gc_malloc(arena, gc_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			copy_quoted_content(str, result, &i, &j);
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}
