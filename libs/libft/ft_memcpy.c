/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 08:55:35 by akivam            #+#    #+#             */
/*   Updated: 2025/06/16 18:21:14 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
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
