/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:29:18 by harici            #+#    #+#             */
/*   Updated: 2026/01/13 20:29:18 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "utils.h"

static char	**create_single_result(char *pattern, t_shell *shell)
{
	char	**result;

	result = gc_malloc(shell->cmd_arena, sizeof(char *) * 2);
	result[0] = gc_strdup(shell->cmd_arena, pattern);
	result[1] = NULL;
	return (result);
}

static char	**process_matches(char *pattern, t_shell *shell)
{
	DIR		*directory;
	char	**result;
	int		count;

	count = count_matches(pattern);
	if (count == 0)
		return (create_single_result(pattern, shell));
	result = gc_malloc(shell->cmd_arena, sizeof(char *) * (count + 1));
	directory = opendir(".");
	if (!directory)
		return (create_single_result(pattern, shell));
	add_matches(directory, pattern, result, shell);
	closedir(directory);
	sort_strings(result, count);
	return (result);
}

char	**expand_wildcard(char *pattern, t_shell *shell)
{
	if (!pattern || !shell)
		return (NULL);
	if (!ft_strchr(pattern, '*'))
		return (create_single_result(pattern, shell));
	return (process_matches(pattern, shell));
}
