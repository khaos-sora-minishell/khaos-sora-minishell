/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 09:41:39 by akivam            #+#    #+#             */
/*   Updated: 2025/09/13 15:15:54 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_word(const char *str, char c)
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

static void	free_split(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static int	fill_words(char **result, const char *s, char c)
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
			result[i] = malloc(len + 1);
			if (!result[i])
				return (free_split(result), -1);
			ft_memcpy(result[i], start, len);
			result[i][len] = '\0';
			i++;
		}
	}
	result[i] = NULL;
	return (0);
}

char	**ft_split(const char *s, char c)
{
	char	**result;
	int		words;

	if (!s)
		return (NULL);
	words = ft_count_word(s, c);
	result = malloc(sizeof(char *) * (words + 1));
	if (!result)
		return (NULL);
	if (fill_words(result, s, c) == -1)
		return (NULL);
	return (result);
}
