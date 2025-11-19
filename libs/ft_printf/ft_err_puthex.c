/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_err_puthex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 18:43:19 by akivam            #+#    #+#             */
/*   Updated: 2025/10/16 18:44:10 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

void	ft_err_puthex(unsigned int number, int *len, char *base)
{
	if (number >= 16)
	{
		ft_err_puthex(number / 16, len, base);
		ft_err_putchar(base[number % 16], len);
	}
	else
		ft_err_putchar(base[number % 16], len);
}
