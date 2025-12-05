/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_memory_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 21:51:42 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:25:14 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

void	*gc_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*tmp;

	i = 0;
	tmp = s;
	while (i < n)
	{
		tmp[i] = (unsigned char)c;
		i = i + 1;
	}
	return (s);
}

void	*gc_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*tmp_src;
	unsigned char	*tmp_dst;

	tmp_src = (unsigned char *)src;
	tmp_dst = (unsigned char *)dest;
	if (n == 0 || tmp_src == tmp_dst)
		return (dest);
	if (tmp_src > tmp_dst)
	{
		i = 0;
		while (i < n)
		{
			tmp_dst[i] = tmp_src[i];
			i++;
		}
	}
	else
	{
		i = n;
		while (i-- > 0)
			tmp_dst[i] = tmp_src[i];
	}
	return (dest);
}

void	*gc_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*tmp_src;
	unsigned char	*tmp_dst;

	i = 0;
	tmp_src = (unsigned char *)src;
	tmp_dst = dest;
	if ((tmp_src == NULL && tmp_dst == NULL) && n > 0)
		return (NULL);
	while (i < n)
	{
		tmp_dst[i] = tmp_src[i];
		i++;
	}
	return (dest);
}

int	gc_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			i;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}

void	*gc_memchr(const void *s1, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;

	i = 0;
	str = (unsigned char *)s1;
	while (i < n)
	{
		if (str[i] == (unsigned char)c)
			return (str + i);
		i++;
	}
	return (NULL);
}
