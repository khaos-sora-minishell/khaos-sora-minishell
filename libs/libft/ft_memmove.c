/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:58:06 by akivam            #+#    #+#             */
/*   Updated: 2025/06/17 18:46:19 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
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
