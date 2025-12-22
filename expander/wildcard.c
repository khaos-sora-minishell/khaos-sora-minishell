/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 04:00:35 by harici            #+#    #+#             */
/*   Updated: 2025/12/22 10:03:00 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "utils.h"

static char	**create_single_result(char *pattern, t_shell *shell)
{
	char	**result;

	result = gc_malloc(shell->cmd_arena, sizeof(char *) * 2);
	result[0] = gc_strdup(shell->cmd_arena, pattern);
	result[1] = NULL;
	return (result);
}

static char	**process_matches(DIR *directory, char *pattern, t_shell *shell)
{
	char	**result;
	int		count;

	count = count_matches(directory, pattern);
	if (count == 0)
	{
		closedir(directory);
		return (create_single_result(pattern, shell));
	}
	result = gc_malloc(shell->cmd_arena, sizeof(char *) * (count + 1));
	add_matches(directory, pattern, result, shell);
	closedir(directory);
	sort_strings(result, count);
	return (result);
}

char	**expand_wildcard(char *pattern, t_shell *shell)
{
	DIR	*directory;

	if (!pattern || !shell)
		return (NULL);
	if (!ft_strchr(pattern, '*'))
		return (create_single_result(pattern, shell));
	directory = opendir(".");
	if (!directory)
		return (NULL);
	return (process_matches(directory, pattern, shell));
}
