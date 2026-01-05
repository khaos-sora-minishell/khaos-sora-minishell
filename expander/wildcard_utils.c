/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 21:16:02 by harici            #+#    #+#             */
/*   Updated: 2026/01/05 21:16:02 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "utils.h"

#ifdef BONUS

int	match_pattern(char *pattern, char *str)
{
	if (!*pattern && !*str)
		return (1);
	if (*pattern == '*')
	{
		if (match_pattern(pattern + 1, str))
			return (1);
		if (*str && match_pattern(pattern, str + 1))
			return (1);
		return (0);
	}
	if (*pattern == *str)
		return (match_pattern(pattern + 1, str + 1));
	return (0);
}

int	count_matches(DIR *directory, char *pattern)
{
	struct dirent	*entry;
	int				count;

	count = 0;
	while (1)
	{
		entry = readdir(directory);
		if (!entry)
			break ;
		if (entry->d_name[0] == '.')
			continue ;
		if (match_pattern(pattern, entry->d_name))
			count++;
	}
	rewinddir(directory);
	return (count);
}

void	add_matches(DIR *directory, char *pattern, char **result,
		t_shell *shell)
{
	struct dirent	*entry;
	int				i;

	i = 0;
	while (1)
	{
		entry = readdir(directory);
		if (!entry)
			break ;
		if (entry->d_name[0] == '.')
			continue ;
		if (match_pattern(pattern, entry->d_name))
		{
			result[i] = gc_strdup(shell->cmd_arena, entry->d_name);
			i++;
		}
	}
	result[i] = NULL;
}

void	sort_strings(char **strings, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(strings[i], strings[j]) > 0)
			{
				temp = strings[i];
				strings[i] = strings[j];
				strings[j] = temp;
			}
			j++;
		}
		i++;
	}
}
#endif
