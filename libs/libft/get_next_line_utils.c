/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:15:00 by akivam            #+#    #+#             */
/*   Updated: 2025/10/12 18:15:55 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft.h"

static char	*init_empty_stash(char **stash, int *should_free)
{
	if (!(*stash))
	{
		*stash = ft_calloc(1, 1);
		*should_free = 1;
	}
	return (*stash);
}

static char	*join_strings(char *stash, const char *buffer, size_t len)
{
	char	*new_str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	new_str = ft_calloc(1, (ft_strlen(stash) + ft_strlen(buffer) + 1));
	if (!new_str)
		return (NULL);
	while (i < len)
	{
		new_str[i] = stash[i];
		i++;
	}
	while (j < ft_strlen(buffer))
		new_str[i++] = buffer[j++];
	new_str[i] = '\0';
	return (new_str);
}

char	*gnl_ft_strjoin(char *stash, const char *buffer)
{
	char	*new_str;
	size_t	len;
	int		should_free;

	should_free = 1;
	if (!init_empty_stash(&stash, &should_free))
		return (NULL);
	if (!stash || !buffer)
		return (NULL);
	len = ft_strlen(stash);
	new_str = join_strings(stash, buffer, len);
	if (!new_str)
		return (NULL);
	if (should_free)
		free(stash);
	return (new_str);
}
