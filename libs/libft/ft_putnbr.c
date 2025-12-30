/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 18:43:19 by akivam            #+#    #+#             */
/*   Updated: 2025/10/16 18:44:54 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr(int n, int *len)
{
	long	number;

	number = n;
	if (number == -2147483648)
	{
		write(1, "-2147483648", 11);
		*len += 11;
		return ;
	}
	if (number < 0)
	{
		ft_putchar('-', len);
		number = -number;
	}
	if (number >= 10)
		ft_putnbr(number / 10, len);
	ft_putchar((number % 10) + '0', len);
}
