/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 18:43:19 by akivam            #+#    #+#             */
/*   Updated: 2025/10/16 18:44:47 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_puthex(unsigned int number, int *len, char *base)
{
	if (number >= 16)
	{
		ft_puthex(number / 16, len, base);
		ft_putchar(base[number % 16], len);
	}
	else
		ft_putchar(base[number % 16], len);
}
