/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:10:57 by akivam            #+#    #+#             */
/*   Updated: 2025/12/07 20:10:58 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

static int	count_words(const char *str, char c)
{
	int	count;
	int	in_word;
	int	i;

	count = 0;
	i = 0;
	in_word = 0;
	while (str[i])
	{
		if (str[i] == c)
			in_word = 0;
		else if (in_word == 0)
		{
			in_word = 1;
			count++;
		}
		i++;
	}
	return (count);
}

static int	fill_words(char **result, const char *s, char c,
	t_gc_context *contex)
{
	int			i;
	const char	*start;
	size_t		len;

	i = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
		{
			start = s;
			while (*s && *s != c)
				s++;
			len = s - start;
			result[i] = gc_malloc(contex, len + 1);
			if (!result[i])
				return (-1);
			gc_memcpy(result[i], start, len);
			result[i][len] = '\0';
			i++;
		}
	}
	result[i] = NULL;
	return (0);
}

char	**gc_split(t_gc_context *contex, const char *s, char c)
{
	char	**result;
	int		words;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	result = gc_malloc(contex, sizeof(char *) * (words + 1));
	if (!result)
		return (NULL);
	if (fill_words(result, s, c, contex) == -1)
		return (NULL);
	return (result);
}
