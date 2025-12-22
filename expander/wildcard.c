/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 04:00:35 by harici            #+#    #+#             */
/*   Updated: 2025/12/22 04:32:44 by harici           ###   ########.fr       */
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

static char	**process_matches(DIR *dir, char *pattern, t_shell *shell)
{
	char	**result;
	int		count;

	count = count_matches(dir, pattern);
	if (count == 0)
	{
		closedir(dir);
		return (create_single_result(pattern, shell));
	}
	result = gc_malloc(shell->cmd_arena, sizeof(char *) * (count + 1));
	add_matches(dir, pattern, result, shell);
	closedir(dir);
	sort_strings(result, count);
	return (result);
}

char	**expand_wildcard(char *pattern, t_shell *shell)
{
	DIR		*dir;

	if (!pattern || !shell)
		return (NULL);
	if (!ft_strchr(pattern, '*'))
		return (create_single_result(pattern, shell));
	dir = opendir(".");
	if (!dir)
		return (NULL);
	return (process_matches(dir, pattern, shell));
}
