/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_err_putnbr_unsigned.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 18:43:19 by akivam            #+#    #+#             */
/*   Updated: 2025/10/16 18:44:14 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

void	ft_err_putnbr_unsigned(unsigned int n, int *len)
{
	unsigned int	number;

	number = n;
	if (number >= 10)
		ft_err_putnbr_unsigned(number / 10, len);
	ft_err_putchar((number % 10) + '0', len);
}
